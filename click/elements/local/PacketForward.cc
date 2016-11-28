#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "PacketForward.hh"
#include <iostream>
#include <clicknet/ip.h>



CLICK_DECLS


PacketForward::PacketForward()
{}

PacketForward::~ PacketForward()
{}

int PacketForward::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, "INFOBASE", cpkM, cpElement, &infoBase, cpEnd) < 0) return -1;
	
	return 0;
}

void PacketForward::push(int s, Packet* p){
	click_ip *iph = (click_ip *)p->data();

	Vector<IPAddress> receivedNetworks = this->infoBase->getNetworks();
	Vector<IPAddress> groups = this->infoBase->getGroups();
	if(iph->ip_dst == IPAddress("224.0.0.1")){
		//All systems querry
		output(0).push(p->clone());
		output(1).push(p->clone());
		return;
	}
	
	States receivedStates = this->infoBase->getStates();
	int i=0;
	for(Vector<IPAddress>::iterator network = receivedNetworks.begin(); network != receivedNetworks.end(); ++network){		        
		if(receivedStates[*network][iph->ip_dst]){
			output(i).push(p->clone());
		}
		i++;
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PacketForward)
