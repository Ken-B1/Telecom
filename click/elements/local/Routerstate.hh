#ifndef CLICK_IGMPROUTERSTATE_HH
#define CLICK_IGMPROUTERSTATE_HH
#include <click/element.hh>
#include <click/hashtable.hh>
#include <iostream>

CLICK_DECLS

//Struct that represents the states
struct State{
  IPAddress group;
  bool status;
};


/*HashMap containing an entry for each attached network
  Each network has a Vector of states attached to it
  This vector contains the IPAddress of the group, and a boolean
  This boolean describes:
    True = include
    False = exclude
  These vectors will be expanded later to add timers
*/

typedef HashTable<IPAddress, Vector<State> > States;

class IGMPRouterState : public Element { 
	public:
		IGMPRouterState();
		~IGMPRouterState();
		
		const char *class_name() const	{ return "IGMPRouterState"; }
		const char *port_count() const	{ return "0/0"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
	private:
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

