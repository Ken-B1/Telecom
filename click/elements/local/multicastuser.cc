#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "multicastuser.hh"
#include <iostream>



CLICK_DECLS

multicastuser::multicastuser()
{}

multicastuser::~ multicastuser()
{}

int multicastuser::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,cpEnd) < 0) return -1;
	return 0;
}

void multicastuser::push(int, Packet* p){
	click_chatter("Sending a request to join network");
	output(0).push(p);
}

int multicastuser::join(const String& conf, Element* e, void * thunk, ErrorHandler * errh){
	multicastuser * me = (multicastuser *) e;
	IPAddress x = 0;
	if(cp_va_kparse(conf, me, errh, "ip", cpkM, cpIPAddress, &x, cpEnd) < 0) return -1;

	//make packet with headroom for ip and ether headers
	//Data contains the data for the igmp packet
	//should start writing this data at an ofset determined by ip and ether header
	int tailroom = 0;
	int headroom = sizeof(click_ip)+sizeof(click_ether);
	int packetsize = sizeof(MulticastMessage);
	WritablePacket* p = Packet::make(headroom, 0, packetsize, tailroom);
	if(p == 0){
	  click_chatter("cannot make packet!");
	  return 0;
	}
	memset(p->data(), 0, p->length());

	MulticastMessage* format = (MulticastMessage*)p->data();

	Record* record = new Record();
	record->RecordType = 3;
	record->AuxDataLen = 0;
	record->NumSources = htons(1);
	record->MulticastAddress = IPAddress(x);
	record->source = IPAddress("192.168.1.1");

	format->Type = 0x22;
	format->Reserved1 = 0;
	format->Checksum = 0;
	format->Reserved2 = 0;
	int16_t numrecords = 0x1;
	format->NumRecords = htons(numrecords);
	format->record = *record;
	
	uint16_t checksum = format->Type + format->NumRecords + format->NumRecords + record->RecordType + ~record->NumSources + record->MulticastAddress + record->source;
	format->Checksum = ~checksum;


        e->push(1, p);	
	return 0; 
}

void multicastuser::add_handlers(){
	add_write_handler("join", &join, (void*)0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(multicastuser)

