#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "MulticastUser.hh"
#include <iostream>



CLICK_DECLS

multicastuser::multicastuser()
{
	this->QRV = 2;
}

multicastuser::~ multicastuser()
{}

int multicastuser::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,"INFOBASE", cpkM, cpElement, &infoBase, cpEnd) < 0) return -1;
	return 0;
}

void multicastuser::push(int, Packet* p){
	output(0).push(p);
}

void multicastuser::send(Packet* p){
	TimerData* timerdata = new TimerData();
	timerdata->me = this;
	timerdata->p = p->clone();
	timerdata->counter = 0;
	
	Timer* t = new Timer(&multicastuser::HandleExpire, timerdata);
	t->initialize(this);
	t->schedule_after_msec(1000);
}

int multicastuser::join(const String& conf, Element* e, void * thunk, ErrorHandler * errh){
	multicastuser * me = (multicastuser *) e;
	IPAddress x = 0;

	if(cp_va_kparse(conf, me, errh, "group", cpkM, cpIPAddress, &x, cpEnd) < 0) return -1;

	me->infoBase->includeGroup(x);
	//Set recordtype to 4 (change_to_exclude_mode) and exclude nothing
	//= Join
	Record* record = new Record();
	record->RecordType = 4;
	record->AuxDataLen = 0;
	record->NumSources = 0;
	record->MulticastAddress = IPAddress(x);

	WritablePacket* p = me->generatePacket();
	MulticastMessage* format = (MulticastMessage*)p->data();
	format->record = *record;


	//Calculate checksum 
	format->Checksum = click_in_cksum((unsigned char *)format, sizeof(MulticastMessage));
	
        me->send(p);	
	return 0; 
}

int multicastuser::leave(const String& conf, Element* e, void * thunk, ErrorHandler * errh){
	multicastuser * me = (multicastuser *) e;
	IPAddress x = 0;
	if(cp_va_kparse(conf, me, errh, "group", cpkM, cpIPAddress, &x, cpEnd) < 0) return -1;

	me->infoBase->excludeGroup(x);

	//Set recordtype to 3 (change_to_include_mode) and exclude nothing
	//= Leave
	Record* record = new Record();
	record->RecordType = 3;
	record->AuxDataLen = 0;
	record->NumSources = 0;
	record->MulticastAddress = IPAddress(x);

	WritablePacket* p = me->generatePacket();
	MulticastMessage* format = (MulticastMessage*)p->data();
	format->record = *record;

	format->Checksum = click_in_cksum((unsigned char *)format, sizeof(MulticastMessage));

        me->send(p);	
	return 0; 
}

WritablePacket* multicastuser::generatePacket(){
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

void multicastuser::HandleExpire(Timer* timer, void* timerdata){
	TimerData* data = (TimerData*)timerdata;
	multicastuser* mu = data->me;
	if(data->counter < mu->getQRV()){
		click_chatter("test");
		data->counter += 1;
		timer->assign(*multicastuser::HandleExpire, data);
		timer->schedule_after_msec(1000);
		mu->push(0, data->p->clone());
	}
}

int multicastuser::getQRV(){
	return this->QRV;
}

void multicastuser::add_handlers(){
	add_write_handler("join", &join, (void*)0);
	add_write_handler("leave", &leave, (void*)0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(multicastuser)

