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
	if (cp_va_kparse(conf, this,errh, "FIRSTNETWORK", cpkM, cpIPAddress, &firstnetwork, "SECONDNETWORK", cpkM, cpIPAddress, &secondnetwork,cpEnd) < 0) return -1;
	this->networks.push_back(firstnetwork);
	this->networks.push_back(secondnetwork);
	return 0;
}


void IGMPRouterState::includeRecord(IPAddress network, IPAddress group){
	this->states[network][group] = true;
	bool checkifnotexists = true;
	for(Vector<IPAddress>::iterator it = this->groups.begin(); it != this->groups.end(); ++it){
		if(*it == group){
			checkifnotexists = false;
		}
	}	
	if(checkifnotexists){
		this->groups.push_back(group);
	}
}

void IGMPRouterState::excludeRecord(IPAddress network, IPAddress group){
	/*Set entry for this combination to false
	*/
	this->states[network][group] = false;
}

String IGMPRouterState::getTextualRepresentation(){
	String returnvalue = "";	
	for(Vector<IPAddress>::iterator it = this->networks.begin(); it != this->networks.end(); ++it){
		returnvalue += "Network: " + it->unparse() + "\n";
		if(this->groups.size() == 0){
			returnvalue += "\t" + String("No groups are active") + "\n";
		}else{
			for(Vector<IPAddress>::iterator groupit = this->groups.begin(); groupit != this->groups.end(); ++groupit){
				returnvalue += "\tGroup " + groupit->unparse() + ": " + String(this->states[*it][*groupit]) + "\n";
			}
		}
	}
	return returnvalue;
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
