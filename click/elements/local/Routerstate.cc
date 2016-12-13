#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "Routerstate.hh"
#include <iostream>



CLICK_DECLS


IGMPRouterState::IGMPRouterState()
{
	this->RobustnessVariable = 2; //Default 2
	this->QueryInterval = 125; //Default 125 seconds
	this->QueryResponseInterval = 100; //Default 100 (10 seconds)
	this->LMQI = 10;
	this->LMQC = this->RobustnessVariable;
}

IGMPRouterState::~ IGMPRouterState()
{}

int IGMPRouterState::configure(Vector<String> &conf, ErrorHandler *errh) {
	IPAddress firstnetwork;
	IPAddress secondnetwork;
	if (cp_va_kparse(conf, this,errh, "FIRSTNETWORK", cpkM, cpIPAddress, &firstnetwork, "SECONDNETWORK", cpkM, cpIPAddress, &secondnetwork, "SOURCEADDRESS", cpkM, cpIPAddress, &srcaddress, cpEnd) < 0) return -1;
	this->networks.push_back(firstnetwork);
	this->networks.push_back(secondnetwork);
	return 0;
}


void IGMPRouterState::excludeRecord(IPAddress network, IPAddress group){
	click_chatter("Received excluderecord");
	//Check if the group already exists to update the ease of use vector
	bool checkifnotexists = true;
	for(Vector<IPAddress>::iterator it = this->groups.begin(); it != this->groups.end(); ++it){
		if(*it == group){
			checkifnotexists = false;
		}
	}	

	if(checkifnotexists){
		//Group doesnt exist yet, so add it
		this->groups.push_back(group);
		
		for(Vector<IPAddress>::iterator it = this->networks.begin(); it != this->networks.end(); ++it){
			StatePerGroup* newgroupstate = new StatePerGroup;
			newgroupstate->multicastAddress = group;
			timergroupdata* tgd = new timergroupdata;
			tgd->spg = newgroupstate;
			tgd->igmprs = this;
			tgd->network = *it;
			newgroupstate->groupTimer = new Timer(&IGMPRouterState::HandleGroupExpire, tgd);
			newgroupstate->groupTimer->initialize(this);

			//Schedule timer after Group membership interval seconds
			int GroupMembershipInterval = (this->RobustnessVariable*this->QueryInterval) + this->QueryResponseInterval/10;
			newgroupstate->groupTimer->schedule_after_sec(GroupMembershipInterval);
			click_chatter(String("Setting group timer to GMI: " + String(GroupMembershipInterval)).c_str());
			if(*it == network){
				newgroupstate->filtermode = true;
			}else{
				newgroupstate->filtermode = false;
			}
			this->states[*it].push_back(newgroupstate);
		}

	}else{
		//Group exists so update it
		groupstate statelist = this->states[network];
		for(groupstate::iterator it = statelist.begin(); it != statelist.end(); ++it){
			if((*it)->multicastAddress == group){
				(*it)->filtermode = true;
				//Update timer to GMI
				int GroupMembershipInterval = (this->RobustnessVariable*this->QueryInterval) + this->QueryResponseInterval/10;
				click_chatter(String("Setting group timer to GMI: " + String(GroupMembershipInterval)).c_str());
				(*it)->groupTimer->clear();
				(*it)->groupTimer->schedule_after_sec(GroupMembershipInterval);
			}
		}
		this->states[network] = statelist;
	}
}

void IGMPRouterState::includeRecord(IPAddress network, IPAddress group){
	//Leaverecord for group is received, so router will querry for other clients possibly in group
	bool checkifnotexists = true;
	for(Vector<IPAddress>::iterator it = this->groups.begin(); it != this->groups.end(); ++it){
		if(*it == group){
			checkifnotexists = false;
		}
	}	

	if(checkifnotexists){
		//Do nothing since setting a group to exclude if it doesnt exist is the same as doing nothing
		return;
	}else{
		/*Send a group querry and update the timers
		  The group timer for this group becomes LMQT = Total time spent after LAST MEMBER QUERY COUNT retransmissions
		  = Last member query interval * last member query count
		*/
		groupstate Groups = this->states[network];
		for(groupstate::iterator it = Groups.begin(); it != Groups.end(); ++it){
			if((*it)->multicastAddress == group){
				double LMQT = this->LMQC * (this->LMQI/10);
				click_chatter(String("Setting group timer to LMQT: " + String(LMQT)).c_str());
				(*it)->groupTimer->clear();
				(*it)->groupTimer->schedule_after_sec(LMQT);
			}
		}
	}
}


void IGMPRouterState::groupExpired(IPAddress network, IPAddress group){
	groupstate Groups = this->states[network];
	for(groupstate::iterator it = Groups.begin(); it != Groups.end(); ++it){
		if((*it)->multicastAddress == group){
			(*it)->filtermode = false;
		}
	}
}


String IGMPRouterState::getTextualRepresentation(){
	String returnvalue = "";	
	for(Vector<IPAddress>::iterator it = this->networks.begin(); it != this->networks.end(); ++it){
		returnvalue += "Network: " + it->unparse() + "\n";
		if(this->groups.size() == 0){
			returnvalue += "\t" + String("No groups are active") + "\n";
		}else{
			groupstate statelist = this->states[*it];
			for(groupstate::iterator groupit = statelist.begin(); groupit != statelist.end(); ++groupit){
				returnvalue += "\tGroup " + (*groupit)->multicastAddress.unparse() + ": " + String((*groupit)->filtermode) + "\n";
			}
		}
	}
	return returnvalue;
}

bool IGMPRouterState::getfiltermode(IPAddress network, IPAddress group){
	groupstate statelist = this->states[network];
	for(groupstate::iterator it = statelist.begin(); it != statelist.end(); ++it){
		if((*it)->multicastAddress == group){
			return (*it)->filtermode;
		}
	}
	return false;
}

Vector<IPAddress> IGMPRouterState::getNetworks(){
	return this->networks;
}

Vector<IPAddress> IGMPRouterState::getGroups(){
	return this->groups;
}

States IGMPRouterState::getStates(){
	return this->states;
}

void IGMPRouterState::HandleGroupExpire(Timer* timer, void* timerdata){
	click_chatter("Group has expired");
	timergroupdata* data = (timergroupdata*)timerdata;
	IGMPRouterState* routerstate = data->igmprs;
	routerstate->groupExpired(data->network, data->spg->multicastAddress);
}


//Below are the handlers for IGMPRouterState
String IGMPRouterState::read(Element* e, void * thunk){
	IGMPRouterState* me = (IGMPRouterState*) e;
	return me->getTextualRepresentation();
}

void IGMPRouterState::add_handlers(){
	add_read_handler("read", &read, (void*)0);
}
CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouterState)
