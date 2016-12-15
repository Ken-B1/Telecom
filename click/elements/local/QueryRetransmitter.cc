#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "QueryRetransmitter.hh"
#include <iostream>



CLICK_DECLS

QueryRetransmitter::QueryRetransmitter(): timer(this)
{}

QueryRetransmitter::~ QueryRetransmitter()
{}

int QueryRetransmitter::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,"INFOBASE", cpkM, cpElement, &infoBase,cpEnd) < 0) return -1;
	timer.initialize(this);
	return 0;
}

void QueryRetransmitter::push(int, Packet* p){
	this->packet = p;
	this->counter = this->infoBase->getLastMemberQueryCount() - 1;
	timer.initialize(this);
	timer.schedule_after_msec(0);
}

void QueryRetransmitter::run_timer(Timer* t){
	click_chatter("Retransmitting a query");
	output(0).push(this->packet->clone());
	if(this->counter > 0){
		counter--;
		double interval = this->infoBase->getLastMemberQueryInterval()/10;
		timer.schedule_after_sec(interval);
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(QueryRetransmitter)

