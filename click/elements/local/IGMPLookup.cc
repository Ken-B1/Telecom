#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "IGMPLookup.hh"
#include <iostream>



CLICK_DECLS

IGMPLookup::IGMPLookup()
{}

IGMPLookup::~ IGMPLookup()
{}

int IGMPLookup::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,"INFOBASE", cpkM, cpElement, &infoBase, cpEnd) < 0) return -1;
	return 0;
}

void IGMPLookup::push(int, Packet* p){

	WritablePacket* q = p->uniqueify();
	click_ip* ipheader = (click_ip*)q->data();
	if(ipheader->ip_p != 2){
		//not an igmp packet, so forward to unicast part
		q->kill();
		output(0).push(p);
	}

	MulticastQuery* Queryheader = (MulticastQuery*)(ipheader + 1);
	IPAddress destination = Queryheader->GroupAddress;
	

	if(this->infoBase->hasGroup(destination)){
		output(1).push(p);
	}else{
		//Packet not of concern to this host, so ignore it
		q->kill();
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPLookup)

