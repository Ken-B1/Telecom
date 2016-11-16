#ifndef CLICK_NULLPULL_HH
#define CLICK_NULLPULL_HH
#include <click/element.hh>
CLICK_DECLS

class Nullpull : public Element { 
	public:
		Nullpull();
		~Nullpull();
		
		const char *class_name() const	{ return "Nullpull"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PULL; }
		int configure(Vector<String>&, ErrorHandler*);
		
		Packet* pull(int);
	private:
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

