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
{}

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


void IGMPRouterState::includeRecord(IPAddress network, IPAddress group){
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
		//Create the sourcerecord for this entry
		sourceList sourcerecord;
		sourcerecord.sourceaddress = this->srcaddress;
		
		for(Vector<IPAddress>::iterator it = this->networks.begin(); it != this->networks.end(); ++it){
			StatePerGroup* newgroupstate = new StatePerGroup;
			newgroupstate->multicastAddress = group;
			newgroupstate->sourcerecord = sourcerecord;
			timergroupdata* tgd = new timergroupdata;
			tgd->spg = newgroupstate;
			tgd->igmprs = this;
			tgd->network = *it;
			newgroupstate->groupTimer = new Timer(&IGMPRouterState::HandleGroupExpire, tgd);
			newgroupstate->groupTimer->initialize(this);
			newgroupstate->groupTimer->schedule_after_sec(10);
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
			}
		}
		this->states[network] = statelist;
	}
}

void IGMPRouterState::excludeRecord(IPAddress network, IPAddress group){
	bool checkifnotexists = true;
	for(Vector<IPAddress>::iterator it = this->groups.begin(); it != this->groups.end(); ++it){
		if(*it == group){
			checkifnotexists = false;
		}
	}	

	if(checkifnotexists){
		//Do nothing since setting a group to exclude if it doesnt exist is the same as doing nothing
	}else{
		/*Set entry for this combination to false
		*/
		groupstate statelist = this->states[network];
		for(groupstate::iterator it = statelist.begin(); it != statelist.end(); ++it){
			if((*it)->multicastAddress == group){
				(*it)->filtermode = false;
			}
		}
		this->states[network] = statelist;
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
	timergroupdata* data = (timergroupdata*)timerdata;
	IGMPRouterState* routerstate = data->igmprs;
	routerstate->excludeRecord(data->network, data->spg->multicastAddress);
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
