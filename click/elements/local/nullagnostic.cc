#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "nullagnostic.hh"

CLICK_DECLS
Nullagnostic::Nullagnostic()
{}

Nullagnostic::~ Nullagnostic()
{}

int Nullagnostic::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this, errh,cpEnd) < 0) return -1;
	return 0;
}

void Nullagnostic::push(int, Packet* p){
	click_chatter("Pushed a packet through agnostic null");
	output(0).push(p);
}

Packet* Nullagnostic::pull(int){
	Packet* p = input(0).pull();
	if(p == 0) return 0;
	click_chatter("Pulled a packet through agnostic null");
	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Nullagnostic)

