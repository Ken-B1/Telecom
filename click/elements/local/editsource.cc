#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "editsource.hh"

CLICK_DECLS
Editsource::Editsource()
{}

Editsource::~ Editsource()
{}

int Editsource::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, cpEnd) < 0) return -1;
	return 0;
}

void Editsource::push(int, Packet* p){
	click_ip* packet = (click_ip*)p->uniqueify();

	/*if(packet->ip_src == ){
	  click_chatter("check");
	}else{
	  click_chatter("done");
	}*/

	click_chatter("Got a packet of size %d",p->length());
	output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Editsource)

