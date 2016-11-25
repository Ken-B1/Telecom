#ifndef CLICK_IGMPROUTERSTATE_HH
#define CLICK_IGMPROUTERSTATE_HH
#include <click/element.hh>
#include <click/hashtable.hh>
#include <iostream>

CLICK_DECLS


/*HashMap containing an entry for each attached network
  Each network has a Hashmap of states attached to it, one state for each group the network is interested in
  This Hashmap contains the MulticastAddress of the group, and a boolean
  This boolean describes:
    True = include
    False = exclude
  These vectors will be expanded later to add timers
*/

typedef HashTable<IPAddress, HashTable<IPAddress, bool> > States;

class IGMPRouterState : public Element { 
	public:
		IGMPRouterState();
		~IGMPRouterState();
		
		const char *class_name() const	{ return "IGMPRouterState"; }
		const char *port_count() const	{ return "0/0"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

		/*Include and exclude records
		  Takes two ipaddresses, the network to update and the groupadress
		*/		
		void includeRecord(IPAddress, IPAddress);
		void excludeRecord(IPAddress, IPAddress);


		/*
		  Handlers for this element
		    Read: gives a textual representation of the current states
		*/
		static String read(Element* e, void* thunk);
		void add_handlers();

		String getTextualRepresentation();

		
	private:
		States states;
		//networks holds all attached networks to simplify lookup and textual printing
		Vector<IPAddress> networks;
		//group holds all existing groups for easy textual representation
		Vector<IPAddress> groups;

		//Check if the group already exists
		bool hasGroup(IPAddress);
};

CLICK_ENDDECLS
#endif

