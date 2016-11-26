#ifndef CLICK_MULTICASTUSER_HH
#define CLICK_MULTICASTUSER_HH
#include <click/element.hh>
#include "PacketDefinitions.hh"
#include "UserinfoBase.hh"

CLICK_DECLS
class multicastuser : public Element { 
	public:
		multicastuser();
		~multicastuser();
		
		const char *class_name() const	{ return "RecordGenerator"; }
		const char *port_count() const	{ return "0/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet *);
		static int join(const String &conf, Element *e, void * thunk, ErrorHandler * errh);
		static int leave(const String &conf, Element *e, void * thunk, ErrorHandler * errh);
		void add_handlers();
	private:
		//Method that generates a packet without any records in it
		WritablePacket* generatePacket();
	
		//Infobase that holds currently joined groups
		UserinfoBase* infoBase;
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

