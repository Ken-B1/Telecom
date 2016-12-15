#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "IGMPQueryChecksum.hh"
#include <iostream>
#include <clicknet/ip.h>



CLICK_DECLS


IGMPQueryChecksum::IGMPQueryChecksum()
{}

IGMPQueryChecksum::~ IGMPQueryChecksum()
{}

int IGMPQueryChecksum::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, cpEnd) < 0) return -1;
	
	return 0;
}

void IGMPQueryChecksum::push(int s, Packet* p){
	WritablePacket *q = p->uniqueify();
	click_ip *iph = (click_ip *)q->data();
	MulticastQuery* Qh = (MulticastQuery*)(iph+1);
	uint16_t Checksum = Qh->Checksum;
	Qh->Checksum = 0;
	uint16_t Controlchecksum = click_in_cksum((unsigned char *)Qh, sizeof(MulticastQuery));
	if(Controlchecksum != Checksum){
		//Packet with incorrect checksum received, so drop packet
		click_chatter("Received a query with an incorrect checksum");
		return;
	}
	output(0).push(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPQueryChecksum)
