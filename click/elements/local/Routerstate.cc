#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>
#include "Routerstate.hh"
#include <iostream>



CLICK_DECLS


IGMPRouterState::IGMPRouterState()
{}

IGMPRouterState::~ IGMPRouterState()
{}

int IGMPRouterState::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (cp_va_kparse(conf, this,errh,cpEnd) < 0) return -1;
	return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(IGMPRouterState)
