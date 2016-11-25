#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "IGMPStateupdate.hh"
#include <iostream>



CLICK_DECLS


IGMPStateupdate::IGMPStateupdate()
{}

IGMPStateupdate::~ IGMPStateupdate()
{}

int IGMPStateupdate::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh, "INFOBASE", cpkM, cpElement, &infoBase, cpEnd) < 0) return -1;
	return 0;
}

void IGMPStateupdate::push(int s, Packet* p){
	click_chatter("Received a multicast report");
	WritablePacket *q = p->uniqueify();

	click_ip* ipHdr = (click_ip*)q->data();
	IPAddress source(ipHdr->ip_src);
	//Bitwise or to set last octet to 254
	source |= IPAddress("0.0.0.254");
	//Bitwise and to make sure last bit is 0
	source &= IPAddress("255.255.255.254");
	MulticastMessage* IGMPmsg = (MulticastMessage*)(ipHdr+1);

	if(IGMPmsg->Type == 0x22){
		//Extract the record from the packet
		Record record = IGMPmsg->record;

		//Update the infobase with this record
		uint8_t recordtype = record.RecordType;
		IPAddress group = record.MulticastAddress;

		switch(recordtype){
			case 3: //Received a include record == leave
				click_chatter("Change to include record has been received");
				//Change names to correctly represent shit
				this->infoBase ->excludeRecord(source, group);
				output(0).push(p);
				break;
			case 4: //Received a exclude record == join
				click_chatter("Change to exclude record has been received");
				this->infoBase->includeRecord(source, group);
				break;

		}
	}else{
		q->kill();
	}
}


CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPStateupdate)
