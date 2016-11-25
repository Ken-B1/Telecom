#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "QueryGenerator.hh"
#include <iostream>



CLICK_DECLS

QueryGenerator::QueryGenerator()
{}

QueryGenerator::~ QueryGenerator()
{}

int QueryGenerator::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,cpEnd) < 0) return -1;
	return 0;
}

void QueryGenerator::push(int, Packet* p){
	click_chatter("Sending a query");

	WritablePacket *q = p->uniqueify();
	click_ip* ipHdr = (click_ip*)q->data();
	MulticastMessage* IGMPmsg = (MulticastMessage*)(ipHdr+1);

	//Check if packet is a state update and if it is a leave
	//Might be redundant but is also more secure

	if(IGMPmsg->Type == 0x22){
		//Extract the record from the packet
		Record record = IGMPmsg->record;

		//Update the infobase with this record
		uint8_t recordtype = record.RecordType;
		IPAddress group = record.MulticastAddress;

		if(recordtype == 3){
			//A include 'Nothing' state update is received (aka a leave), so query other members of group for status
			//make packet with headroom for ip and ether headers
			//Data contains the data for the igmp packet
			//should start writing this data at an ofset determined by ip and ether header
			int tailroom = 0;
			int headroom = sizeof(click_ip)+sizeof(click_ether);
			int packetsize = sizeof(MulticastQuery);
			WritablePacket* Query = Packet::make(headroom, 0, packetsize, tailroom);
			if(Query == 0){
			  click_chatter("cannot make packet!");
			  return;
			}


			memset(Query->data(), 0, Query->length());

			MulticastQuery* format = (MulticastQuery*)Query->data();
			//Fill Querypacket with correct information
			format->Type = 0x11;
			format->MRC = 0;
			format->Checksum = 0;
			format->GroupAddress = record.MulticastAddress;
			format->Reserved = 0;
			format->Suppress = false;
			format->QRV = 0;
			format->QQIC = 0;
			format->NumSources = htons(1);
			format->Source = record.source;

			output(0).push(Query);
		}else{
			//Packet is not a leave update report packet, so no action should be taken
			return;

		}
	}
}

/*
int QueryGenerator::join(const String& conf, Element* e, void * thunk, ErrorHandler * errh){
	multicastuser * me = (multicastuser *) e;
	IPAddress x = 0;

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

	//Set recordtype to 4 (change_to_exclude_mode) and exclude nothing
	//= Join
	Record* record = new Record();
	record->RecordType = 4;
	record->AuxDataLen = 0;
	record->NumSources = 0;
	record->MulticastAddress = IPAddress(x);

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
*/

CLICK_ENDDECLS
EXPORT_ELEMENT(QueryGenerator)

