#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "nullpull.hh"

CLICK_DECLS
Nullpull::Nullpull()
{}

Nullpull::~ Nullpull()
{}

int Nullpull::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this, errh,cpEnd) < 0) return -1;
	return 0;
}

Packet* Nullpull::pull(int){
	Packet* p = input(0).pull();
	if(p == 0) return 0;
	click_chatter("Pulled a packet through null");
	return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Nullpull)

