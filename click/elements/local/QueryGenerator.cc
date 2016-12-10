#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "QueryGenerator.hh"
#include <iostream>



CLICK_DECLS

QueryGenerator::QueryGenerator(): timer(this)
{}

QueryGenerator::~ QueryGenerator()
{}

int QueryGenerator::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,cpEnd) < 0) return -1;
	timer.initialize(this);
	timer.schedule_after_msec(1000);
	return 0;
}

void QueryGenerator::push(int, Packet* p){

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

		//Generate the Supress and QRV parameters
		bool Suppress = 0;
		int QRV = 2; //Should always be smaller than 8

		//Shift suppress 3 bits to the left
		uint8_t resvsqvr = ((int)Suppress << 3) + QRV;


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
			format->GroupAddress = group;
			format->ResvSQvr = resvsqvr;
			format->QQIC = 0;
			format->NumSources = htons(1);
			format->Source = record.source;

			format->Checksum = click_in_cksum((unsigned char *)format, sizeof(MulticastQuery));

			output(0).push(Query);
		}else{
			//Packet is not a leave update report packet, so no action should be taken
			return;

		}
	}
}

void QueryGenerator::run_timer(Timer* timer){
	//Create packet
	int tailroom = 0;
	int headroom = sizeof(click_ip)+sizeof(click_ether);
	int packetsize = sizeof(MulticastQuery);
	WritablePacket* Query = Packet::make(headroom, 0, packetsize, tailroom);
	if(Query == 0){
	  click_chatter("cannot make packet!");
	  return;
	}


	memset(Query->data(), 0, Query->length());

	//Generate the Supress and QRV parameters
	bool Suppress = 0;
	int QRV = 2; //Should always be smaller than 8

	//Shift suppress 3 bits to the left
	uint8_t resvsqvr = ((int)Suppress << 3) + QRV;

	MulticastQuery* format = (MulticastQuery*)Query->data();
	//Fill Querypacket with correct information
	format->Type = 0x11;
	format->MRC = 0;
	format->Checksum = 0;
	format->GroupAddress = IPAddress("0.0.0.0");
	format->ResvSQvr = resvsqvr;
	format->QQIC = 0;
	format->NumSources = htons(1);
	format->Source = 0;

	format->Checksum = click_in_cksum((unsigned char *)format, sizeof(MulticastQuery));

	//Push query to global query output
	output(1).push(Query);

	timer->schedule_after_msec(1000);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(QueryGenerator)

