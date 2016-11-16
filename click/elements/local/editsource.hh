#ifndef CLICK_EDITSOURCE_HH
#define CLICK_EDITSOURCE_HH
#include <click/element.hh>

CLICK_DECLS
class Editsource : public Element { 
	public:
		Editsource();
		~Editsource();
		
		const char *class_name() const	{ return "EditSource"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet *);
	private:
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

