#ifndef CLICK_IGMPROUTERSTATE_HH
#define CLICK_IGMPROUTERSTATE_HH
#include <click/element.hh>
#include <click/hashtable.hh>
#include <click/timer.hh>
#include <iostream>

CLICK_DECLS
/*Struct that contains a sourceaddress with a sourcetimer
  For our implementation this is quite useless (since there is only one source), 
  but we still implement it to adhere closely to the rfc
  The struct contains the sourceaddress and the sourcetimer
  This sourceaddress will always be the sourceaddress of the singular server
*/

struct sourceList{
	IPAddress sourceaddress;
	//Timer sourcetimer;
};

/*Struct that contains a router state-per-group
  This struct will be used to maintain the state-per-group-per-attachet-network
  It contains:
    multicastAddress -> group address
    groupTimer -> grouptimer
    filtermode -> the filtermode. It works as follows:
      True = include
      False = exclude
    sourcelist -> The sourcelist (or in our case sourceelement) that represents the requested sources
*/
struct StatePerGroup{
	IPAddress multicastAddress;
	Timer* groupTimer;
	bool filtermode;
	//sourceList sourcerecord;
};

/*HashMap containing an entry for each attached network
  Each network has a Hashmap of states attached to it, one state for each group the network is interested in
  This Hashmap contains the MulticastAddress of the group, and a boolean
  This boolean describes:
    True = include
    False = exclude
  These vectors will be expanded later to add timers
*/

typedef Vector<StatePerGroup*> groupstate;
typedef HashTable<IPAddress, groupstate> States;

class IGMPRouterState : public Element { 
	public:
		IGMPRouterState();
		~IGMPRouterState();
		
		const char *class_name() const	{ return "IGMPRouterState"; }
		const char *port_count() const	{ return "0/0"; }
		int configure(Vector<String>&, ErrorHandler*);

		/*Include and exclude records
		  Takes two ipaddresses, the network to update and the groupadress
		  The groupExpired method will be used to actually set a group filtermode to include
		  This happens after a timer runs out
		  The includeRecord will not set a group to include, but will execute the correct actions according to the rfc
		*/		
		void includeRecord(IPAddress, IPAddress);
		void excludeRecord(IPAddress, IPAddress);
		void groupExpired(IPAddress, IPAddress);

		/*
		  Handlers for this element
		    Read: gives a textual representation of the current states
		*/
		static String read(Element* e, void* thunk);
		void add_handlers();

		String getTextualRepresentation();
		States getStates();
		Vector<IPAddress> getGroups();
		Vector<IPAddress> getNetworks();

		//Method to request filtermode for network x and group y
		bool getfiltermode(IPAddress, IPAddress);

		//Methods for timing
		static void HandleGroupExpire(Timer*, void*);
		
	private:
		//Group Membership Interval = Robustnessvariable * Queryinterval + Query response interval
		//Different settings used in querries and other igmp functions
		int RobustnessVariable; //Default 2
		int QueryInterval; //Default 125 seconds
		int QueryResponseInterval; //Default 100 (10 seconds)
		double LMQI; //Default 10 (1 second)
		double LMQC; //Default RobustnessVariable
		States states;
		//networks holds all attached networks to simplify lookup and textual printing
		Vector<IPAddress> networks;
		//group holds all existing groups for easy textual representation
		Vector<IPAddress> groups;
		//Sourceaddress of our server
		IPAddress srcaddress;

		//Check if the group already exists
		bool hasGroup(IPAddress);

		//TimerGroupData
		struct timergroupdata{
			IPAddress network;
			StatePerGroup* spg;
			IGMPRouterState* igmprs; 
		};
};

CLICK_ENDDECLS
#endif

