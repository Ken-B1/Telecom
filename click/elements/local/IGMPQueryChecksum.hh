#ifndef CLICK_IGMPQUERYCHECKSUM_HH
#define CLICK_IGMPQUERYCHECKSUM_HH
#include <click/element.hh>
#include <iostream>
#include "Routerstate.hh"
#include "PacketDefinitions.hh"

CLICK_DECLS

class IGMPQueryChecksum : public Element { 
	public:
		IGMPQueryChecksum();
		~IGMPQueryChecksum();
		
		const char *class_name() const	{ return "IGMPQueryChecksum"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		void push(int, Packet*);

	private:
};

CLICK_ENDDECLS
#endif

