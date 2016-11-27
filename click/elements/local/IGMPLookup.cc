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
		if(ipheader->ip_p != 17){
			//not an igmp packet or source data packet so forward to unicast part
			q->kill();
			output(0).push(p);
		}else{
			//Udppacket with data from source
			//Check if packet is destined for this client, else ignore it
			if(this->infoBase->hasGroup(ipheader->ip_dst)){
				//Push to output 3 (udp datapacket from multicast source) destined for this client
				q->kill();
				output(2).push(p);				
			}else{
				//Multicast data packet that isnt destined for this client
				q->kill();
				return;
			}
		}
	}

	MulticastQuery* Queryheader = (MulticastQuery*)(ipheader + 1);
	IPAddress destination = Queryheader->GroupAddress;
	

	if(this->infoBase->hasGroup(destination)){
		output(1).push(p);
	}else if(ipheader->ip_dst == IPAddress("224.0.0.1")){
		//Query to all systems, so accept
		output(1).push(p);
	}else{
		//Packet not of concern to this host, so ignore it
		q->kill();
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPLookup)

