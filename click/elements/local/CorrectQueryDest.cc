#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "CorrectQueryDest.hh"
#include <iostream>
#include <clicknet/ip.h>



CLICK_DECLS


CorrectQueryDest::CorrectQueryDest()
{}

CorrectQueryDest::~ CorrectQueryDest()
{}

int CorrectQueryDest::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, cpEnd) < 0) return -1;
	return 0;
}

void CorrectQueryDest::push(int s, Packet* p){
	WritablePacket *q = p->uniqueify();
	click_ip *iph = (click_ip *)q->data();
	MulticastQuery* mch = (MulticastQuery*)(iph+1);
	IPAddress group = mch->GroupAddress;
	iph->ip_dst = group;
	iph->ip_sum = 0; 
	iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip) + sizeof(MulticastQuery));
	output(0).push(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CorrectQueryDest)
