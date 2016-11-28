#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "PacketReroute.hh"
#include <iostream>
#include <clicknet/ip.h>



CLICK_DECLS


PacketReroute::PacketReroute()
{}

PacketReroute::~ PacketReroute()
{}

int PacketReroute::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, "INFOBASE", cpkM, cpElement, &infoBase, cpEnd) < 0) return -1;
	
	return 0;
}

void PacketReroute::push(int s, Packet* p){
	
	Vector<IPAddress> receivedGroups = this->infoBase->getGroups();
	bool sentPackage=false;	

	for(Vector<IPAddress>::iterator group = receivedGroups.begin(); group != receivedGroups.end(); ++group){		        
		WritablePacket *q = p->uniqueify();
		click_ip *iph = (click_ip *)q->data();
		iph->ip_dst = *group;
		sentPackage=true;
		output(0).push(q);
	}
	
	if(!sentPackage){
		output(0).push(p);
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PacketReroute)
