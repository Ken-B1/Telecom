#ifndef CLICK_NULLPUSH_HH
#define CLICK_NULLPUSH_HH
#include <click/element.hh>
CLICK_DECLS

class Nullpush : public Element { 
	public:
		Nullpush();
		~Nullpush();
		
		const char *class_name() const	{ return "Nullpush"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
		void push(int, Packet *);
	private:
		uint32_t maxSize;
};

CLICK_ENDDECLS
#endif

