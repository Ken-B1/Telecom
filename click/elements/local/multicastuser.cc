#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "multicastuser.hh"

CLICK_DECLS
multicastuser::multicastuser()
{}

multicastuser::~ multicastuser()
{}

int multicastuser::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, cpEnd) < 0) return -1;
	return 0;
}

void multicastuser::push(int, Packet* p){
	click_ip* packet = (click_ip*)p->uniqueify();
	click_chatter("Got a packet of size %d",p->length());
	output(0).push(p);
}

int multicastuser::join(const String& conf, Element* e, void * thunk, ErrorHandler * errh){
	multicastuser * me = (multicastuser *) e;
	int x = 0;
	if(cp_va_kparse(conf, me, errh, "ip", cpkM, cpInteger, &x, cpEnd) < 0) return -1;
	
	return 0; 
}

void multicastuser::add_handlers(){
	add_write_handler("join", &join, (void*)0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(multicastuser)

