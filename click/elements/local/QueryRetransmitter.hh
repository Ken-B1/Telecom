#ifndef CLICK_QUERYRETRANSMITTER_HH
#define CLICK_QUERYRETRANSMITTER_HH
#include <click/element.hh>
#include "PacketDefinitions.hh"
#include "Routerstate.hh"
#include <click/timer.hh>

CLICK_DECLS
class QueryRetransmitter : public Element { 
	public:
		QueryRetransmitter();
		~QueryRetransmitter();
		
		const char *class_name() const	{ return "QueryRetransmitter"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet*);
		void run_timer(Timer*);
	private:
		IGMPRouterState* infoBase;
		//Timer for general querries
		Timer timer;
		//Packet to be retransmitted
		Packet* packet;
		//Counter for how many times this packet has been retransmitted
		int counter;
};

CLICK_ENDDECLS
#endif

