#ifndef CLICK_CORRECTQUERYDEST_HH
#define CLICK_CORRECTQUERYDEST_HH
#include <click/element.hh>
#include <iostream>
#include "Routerstate.hh"
#include "PacketDefinitions.hh"

CLICK_DECLS

class CorrectQueryDest : public Element { 
	public:
		CorrectQueryDest();
		~CorrectQueryDest();
		
		const char *class_name() const	{ return "CorrectQueryDest"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		void push(int, Packet*);

	private:
};

CLICK_ENDDECLS
#endif

