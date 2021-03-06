#ifndef CLICK_IPSECAUTHSHA1_HH
#define CLICK_IPSECAUTHSHA1_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <click/glue.hh>
CLICK_DECLS

/*
 * =c
 * IPsecAuthSHA1(VERIFY)
 * =s ipsec
 * verify SHA1 authentication digest.
 * =d
 *
 * If first argument is 0, computes SHA1 authentication digest for ESP packet
 * per RFC 2404, 2406. If first argument is 1, verify SHA1 digest and remove
 * authentication bits.
 *
 * =a IPsecESPEncap, IPsecDES, IPsecAES
 */

class IPsecAuthSHA1 : public Element {

public:
  IPsecAuthSHA1();
  ~IPsecAuthSHA1();

  const char *class_name() const	{ return "IPsecAuthSHA1"; }
  const char *port_count() const	{ return "1/-"; }

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);

  Packet *simple_action(Packet *);
  void add_handlers();

  static String drop_handler(Element *e, void *thunk);

private:

  int _op;
  atomic_uint32_t _drops;
  enum { COMPUTE_AUTH = 0, VERIFY_AUTH = 1 };
};

CLICK_ENDDECLS
#endif
