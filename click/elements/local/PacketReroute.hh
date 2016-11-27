#ifndef CLICK_PACKETREROUTE_HH
#define CLICK_PACKETREROUTE_HH
#include <click/element.hh>
#include <iostream>
#include "Routerstate.hh"
#include "PacketDefinitions.hh"

CLICK_DECLS

class PacketReroute : public Element { 
	public:
		PacketReroute();
		~PacketReroute();
		
		const char *class_name() const	{ return "PacketReroute"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		void push(int, Packet*);

	private:
		IGMPRouterState* infoBase;
};

CLICK_ENDDECLS
#endif

