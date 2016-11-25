#ifndef CLICK_IGMPSTATEUPDATE_HH
#define CLICK_IGMPSTATEUPDATE_HH
#include <click/element.hh>
#include <click/hashtable.hh>
#include <iostream>
#include "Routerstate.hh"
#include "PacketDefinitions.hh"

CLICK_DECLS


class IGMPStateupdate : public Element { 
	public:
		IGMPStateupdate();
		~IGMPStateupdate();
		
		const char *class_name() const	{ return "IGMPStateupdate"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		void push(int, Packet*);
		
	private:
		IGMPRouterState* infoBase;
		
};

CLICK_ENDDECLS
#endif

