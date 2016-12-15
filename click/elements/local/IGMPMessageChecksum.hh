#ifndef CLICK_IGMPMESSAGECHECKSUM_HH
#define CLICK_IGMPMESSAGECHECKSUM_HH
#include <click/element.hh>
#include <iostream>
#include "Routerstate.hh"
#include "PacketDefinitions.hh"

CLICK_DECLS

class IGMPMessageChecksum : public Element { 
	public:
		IGMPMessageChecksum();
		~IGMPMessageChecksum();
		
		const char *class_name() const	{ return "IGMPMessageChecksum"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		void push(int, Packet*);

	private:
};

CLICK_ENDDECLS
#endif

