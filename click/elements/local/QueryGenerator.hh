#ifndef CLICK_QUERYGENERATOR_HH
#define CLICK_QUERYGENERATOR_HH
#include <click/element.hh>
#include "PacketDefinitions.hh"
#include <click/timer.hh>

CLICK_DECLS
class QueryGenerator : public Element { 
	public:
		QueryGenerator();
		~QueryGenerator();
		
		const char *class_name() const	{ return "QueryGenerator"; }
		const char *port_count() const	{ return "1/2"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet*);
		void run_timer(Timer*);
	private:
		uint32_t maxSize;
		//Timer for general querries
		Timer timer;
};

CLICK_ENDDECLS
#endif

