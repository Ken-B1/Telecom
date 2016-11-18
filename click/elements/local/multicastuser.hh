#ifndef CLICK_MULTICASTUSER_HH
#define CLICK_MULTICASTUSER_HH
#include <click/element.hh>

CLICK_DECLS
class multicastuser : public Element { 
	public:
		multicastuser();
		~multicastuser();
		
		const char *class_name() const	{ return "multicastuser"; }
		const char *port_count() const	{ return "0/0"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet *);
		static int join(const String &conf, Element *e, void * thunk, ErrorHandler * errh);
		void add_handlers();
	private:
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

