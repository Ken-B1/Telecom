#ifndef CLICK_IGMPLOOKUP_HH
#define CLICK_IGMPLOOKUP_HH
#include <click/element.hh>
#include "PacketDefinitions.hh"
#include "UserinfoBase.hh"

CLICK_DECLS
class IGMPLookup : public Element { 
	//Class that routes igmp packets of concern to this client to output 1
	//All other packets will be put on output 0
	public:
		IGMPLookup();
		~IGMPLookup();
		
		const char *class_name() const	{ return "IGMPLookup"; }
		const char *port_count() const	{ return "1/2"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet *);

	private:
		//Infobase that holds currently joined groups
		UserinfoBase* infoBase;
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

