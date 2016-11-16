#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "nullpush.hh"

CLICK_DECLS
Nullpush::Nullpush()
{}

Nullpush::~ Nullpush()
{}

int Nullpush::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this, errh,cpEnd) < 0) return -1;
	return 0;
}

void Nullpush::push(int, Packet *p){
	click_chatter("Got a packet of size %d",5);
	output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Nullpush)

