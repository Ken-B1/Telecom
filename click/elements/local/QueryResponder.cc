#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "QueryResponder.hh"
#include <iostream>



CLICK_DECLS

QueryResponder::QueryResponder()
{}

QueryResponder::~ QueryResponder()
{}

int QueryResponder::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,"INFOBASE", cpkM, cpElement, &infoBase, cpEnd) < 0) return -1;
	return 0;
}

void QueryResponder::push(int, Packet* p){
	WritablePacket* q = p->uniqueify();
	Groups usergroups = this->infoBase->getstates();
	click_ip* ipheader = (click_ip*)q->data();
	MulticastQuery* Queryheader = (MulticastQuery*)(ipheader+1);
	IPAddress group = Queryheader->GroupAddress;

	for(Groups::iterator it = usergroups.begin(); it != usergroups.end(); ++it){			
		//Set recordtype to 2 (MODE_IS_EXCLUDE) and exclude nothing
		//= "Join" or more specifically, confirm that this user is still in the group
		Record* queryrecord = new Record();
		queryrecord->RecordType = 2;
		queryrecord->AuxDataLen = 0;
		queryrecord->NumSources = 0;
		queryrecord->MulticastAddress = *it;

		WritablePacket* query = this->generatePacket();
		MulticastMessage* format = (MulticastMessage*)query->data();
		format->record = *queryrecord;

		format->Checksum = click_in_cksum((unsigned char *)format, sizeof(MulticastMessage));	

		output(0).push(query);
	}
}

WritablePacket* QueryResponder::generatePacket(){
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

	//Set recordtype to 3 (change_to_include_mode) and exclude nothing
	//= Leave

	format->Type = 0x22;
	format->Reserved1 = 0;
	format->Checksum = 0;
	format->Reserved2 = 0;
	int16_t numrecords = 0x1;
	format->NumRecords = htons(numrecords);


	return p;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(QueryResponder)

