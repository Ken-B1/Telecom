#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "QueryResponder.hh"
#include <iostream>



CLICK_DECLS

QueryResponder::QueryResponder(): general_timer(this), group_timer(this)
{}

QueryResponder::~ QueryResponder()
{}

int QueryResponder::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,"INFOBASE", cpkM, cpElement, &infoBase, cpEnd) < 0) return -1;
	general_timer.initialize(this);
	group_timer.initialize(this);
	general_timer_value=0;
	group_timer_value=0;
	return 0;
}

void QueryResponder::push(int, Packet* p){
	WritablePacket *q = p->uniqueify();
	click_ip* ipHdr = (click_ip*)q->data();

	MulticastQuery* incQuery = (MulticastQuery*)(ipHdr+1);
	uint8_t MRC = incQuery->MRC;
	int temp_time=rand() % MRC + 1;

	if(incQuery->GroupAddress==NULL){	
		if(general_timer.scheduled()){
			general_timer.unschedule();
		}	
		if(temp_time < general_timer_value || general_timer_value==0){		
			general_packet = q;	
			general_timer_value = temp_time;	
			general_timer.schedule_after_sec(general_timer_value);
		}
	} else {
		if(group_timer.scheduled()){
			group_timer.unschedule();
			group_packet = q;	
			if(temp_time<group_timer_value){
				group_timer_value = temp_time;	
				group_timer.schedule_after_sec(group_timer_value);
			} else {
				group_timer.schedule_after_sec(group_timer_value);
			}
		} else {
			group_packet = q;	
			group_timer_value = temp_time;	
			group_timer.schedule_after_sec(group_timer_value);
		}	
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

void QueryResponder::run_timer(Timer* timer){
	if(timer == &group_timer){
		click_chatter("group_timer");

		click_ip* ipheader = (click_ip*)group_packet->data();
		MulticastQuery* Queryheader = (MulticastQuery*)(ipheader+1);
		IPAddress group = Queryheader->GroupAddress;
			
		//Set recordtype to 2 (MODE_IS_EXCLUDE) and exclude nothing
		//= "Join" or more specifically, confirm that this user is still in the group
		Record* queryrecord = new Record();
		queryrecord->RecordType = 2;
		queryrecord->AuxDataLen = 0;
		queryrecord->NumSources = 0;
		queryrecord->MulticastAddress = group;

		WritablePacket* query = this->generatePacket();
		MulticastMessage* format = (MulticastMessage*)query->data();
		format->record = *queryrecord;

		format->Checksum = click_in_cksum((unsigned char *)format, sizeof(MulticastMessage));	
		
		output(0).push(query);	
		group_timer_value=0;	
	} else {
		click_chatter("general_timer");

		Groups usergroups = this->infoBase->getstates();
		click_ip* ipheader = (click_ip*)general_packet->data();
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

		general_timer_value=0;
	}	
	click_chatter("Running timer");
}


CLICK_ENDDECLS
EXPORT_ELEMENT(QueryResponder)

