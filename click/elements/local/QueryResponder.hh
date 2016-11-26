#ifndef CLICK_QUERYRESPONDER_HH
#define CLICK_QUERYRESPONDER_HH
#include <click/element.hh>
#include "PacketDefinitions.hh"

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
	private:
		//Method that generates a packet without any records in it
		WritablePacket* generatePacket();
};

CLICK_ENDDECLS
#endif

