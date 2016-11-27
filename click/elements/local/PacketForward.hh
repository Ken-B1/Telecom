#ifndef CLICK_PACKETFORWARD_HH
#define CLICK_PACKETFORWARD_HH
#include <click/element.hh>
#include <iostream>
#include "Routerstate.hh"
#include "PacketDefinitions.hh"

CLICK_DECLS

class PacketForward : public Element { 
	public:
		PacketForward();
		~PacketForward();
		
		const char *class_name() const	{ return "PacketForward"; }
		const char *port_count() const	{ return "1/2"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		void push(int, Packet*);

	private:
		IGMPRouterState* infoBase;
};

CLICK_ENDDECLS
#endif

