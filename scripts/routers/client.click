// Output configuration: 
//
// Packets for the network are put on output 0
// Packets for the host are put on output 1

elementclass Client {
	$address, $gateway |

	//Infobase element for the multicast states
	infoBase :: UserinfoBase();


        //Element that will instantiate a join or leave in the multicast network
	user :: RecordGenerator(INFOBASE infoBase)
		-> IPEncap(2, $address, multicast_report_address, TTL 1)
		-> EtherEncap(0x0800, $address, $gateway)
		-> output;

	ip :: Strip(14)
		-> CheckIPHeader()
		-> rt :: StaticIPLookup(
					$address:ip/32 0,
					$address:ipnet 0,
					0.0.0.0/0.0.0.0 $gateway 1,
					224.0.0.0/8 2)
		-> [1]output;
	
	rt[1]
		-> DropBroadcasts
		-> ipgw :: IPGWOptions($address)
		-> FixIPSrc($address)
		-> ttl :: DecIPTTL
		-> frag :: IPFragmenter(1500)
		-> arpq :: ARPQuerier($address)
		-> output;

	//Responder to querys
	rt[2]
		-> [1]output;

	ipgw[1]
		-> ICMPError($address, parameterproblem)
		-> output;
	
	ttl[1]
		-> ICMPError($address, timeexceeded)
		-> output; 

	frag[1]
		-> ICMPError($address, unreachable, needfrag)
		-> output;

	// Incoming Packets
	input
		-> QueryResponder
		-> HostEtherFilter($address)  //Filter packets not intended for this host
		-> in_cl :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800)  //(arp requests, arp replies, ip packets)
		-> arp_res :: ARPResponder($address)
		-> output;

	in_cl[1]
		-> [1]arpq;
	
	in_cl[2]
		-> ip;
}
