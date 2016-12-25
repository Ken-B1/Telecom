#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "UserRetransmitter.hh"
#include <iostream>



CLICK_DECLS

UserRetransmitter::UserRetransmitter(): timer(this)
{}

UserRetransmitter::~ UserRetransmitter()
{}

int UserRetransmitter::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,"INFOBASE", cpkM, cpElement, &infoBase,cpEnd) < 0) return -1;
	timer.initialize(this);
	click_chatter(String(infoBase->getQRV()).c_str());
	return 0;
}

void UserRetransmitter::push(int, Packet* p){
	this->packet = p;
	this->counter = this->infoBase->getQRV() - 1;
	click_chatter(String(this->counter).c_str());
	timer.initialize(this);
	timer.schedule_after_msec(0);
}

void UserRetransmitter::run_timer(Timer* t){
	click_chatter("Retransmitting a user datagram");
	output(0).push(this->packet->clone());
	if(this->counter > 0){
		click_chatter(String(this->counter).c_str());
		counter--;
		//double interval = this->infoBase->getLastMemberQueryInterval()/10;
		timer.schedule_after_sec(1);
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(UserRetransmitter)

