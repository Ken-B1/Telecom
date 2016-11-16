#ifndef CLICK_NULLAGNOSTIC_HH
#define CLICK_NULLAGNOSTIC_HH
#include <click/element.hh>
CLICK_DECLS

class Nullagnostic : public Element { 
	public:
		Nullagnostic();
		~Nullagnostic();
		
		const char *class_name() const	{ return "Nullagnostic"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return AGNOSTIC; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet*);
		Packet* pull(int);
	private:
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

