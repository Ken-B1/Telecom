#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "UserinfoBase.hh"
#include <iostream>



CLICK_DECLS


UserinfoBase::UserinfoBase()
{
	this->QRV = 2;
	this->URI = 1;
}

UserinfoBase::~ UserinfoBase()
{}

int UserinfoBase::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, cpEnd) < 0) return -1;
	return 0;
}


void UserinfoBase::includeGroup(IPAddress group){
	//= exclude an empty source list, aka join a group and receive all sources
	if(!this->hasGroup(group)){
		this->states.push_back(group);
	}
}

void UserinfoBase::excludeGroup(IPAddress group){
	//= include an empty source list, aka leave a group
	if(this->hasGroup(group)){
		for(Groups::iterator it = this->states.begin(); it != this->states.end(); ++it){
			if(group == *it){
				this->states.erase(it);
				break;
			}
		} 
	}
}

Groups UserinfoBase::getstates(){
	return this->states;
}

int UserinfoBase::getQRV(){
	click_chatter(String("Called for qrv: " + String(this->QRV)).c_str());
	return this->QRV;
}

double UserinfoBase::getURI(){
	return this->URI;
}

String UserinfoBase::getTextualRepresentation(){
	String returnvalue = "";	
	for(Groups::iterator it = this->states.begin(); it != this->states.end(); ++it){
		returnvalue += it->unparse() + "\n";
	}
	click_chatter(returnvalue.c_str());
	return returnvalue;
}


//Below are the handlers for IGMPRouterState
String UserinfoBase::read(Element* e, void * thunk){
	UserinfoBase* me = (UserinfoBase*) e;
	return me->getTextualRepresentation();
}

void UserinfoBase::add_handlers(){
	add_read_handler("read", &read, (void*)0);
}

bool UserinfoBase::hasGroup(IPAddress group){
	for(Groups::iterator it = this->states.begin(); it != this->states.end(); ++it){
		if(group == *it){
			return true;
		}
	}
	return false;
}
CLICK_ENDDECLS
EXPORT_ELEMENT(UserinfoBase)
