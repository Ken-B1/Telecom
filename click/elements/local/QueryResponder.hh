#ifndef CLICK_QUERYRESPONDER_HH
#define CLICK_QUERYRESPONDER_HH
#include <click/element.hh>
#include "PacketDefinitions.hh"
#include "UserinfoBase.hh"
#include <click/timer.hh>

CLICK_DECLS
class QueryResponder : public Element { 
	public:
		QueryResponder();
		~QueryResponder();
		
		const char *class_name() const	{ return "QueryResponder"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet *);
		void run_timer(Timer*);
	private:
		//Method that generates a packet without any records in it
		UserinfoBase* infoBase;
		WritablePacket* generatePacket();
		//Timer for general querries
		Timer general_timer;
		int general_timer_value;
		WritablePacket* general_packet;
		//Timer for group querries
		Timer group_timer;
		int group_timer_value;
		WritablePacket* group_packet;
};

CLICK_ENDDECLS
#endif

