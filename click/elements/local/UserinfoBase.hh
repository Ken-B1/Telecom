#ifndef CLICK_USERINFOBASE_HH
#define CLICK_USERINFOBASE_HH
#include <click/element.hh>
#include <click/hashtable.hh>
#include <iostream>

CLICK_DECLS


//Vector that contains all the groups this client is part of

typedef Vector<IPAddress> Groups;

class UserinfoBase : public Element { 
	public:
		UserinfoBase();
		~UserinfoBase();
		
		const char *class_name() const	{ return "UserinfoBase"; }
		const char *port_count() const	{ return "0/0"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

		/*Updates membership of group 
		*/		
		void includeGroup(IPAddress);
		void excludeGroup(IPAddress);


		/*
		  Handlers for this element
		    Read: gives a textual representation of the current states
		*/
		static String read(Element* e, void* thunk);
		void add_handlers();

		String getTextualRepresentation();

		
	private:
		Groups states;

		//Check if the group already exists
		bool hasGroup(IPAddress);
};

CLICK_ENDDECLS
#endif

