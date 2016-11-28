# Telecom
Implemented:
	- Clients can send join and leave messages
	- Router can process these messages and forward server packets to the correct users

Usage:
	To command the clients, you can use telnet to call handlers
	The group used to send packets to is 224.0.0.15


	clients have 2 handlers, a join handler and a leave handler
	
	write clientxy/user.join group 224.0.0.15	
	write clientxy/user.leave group 224.0.0.15

	clients and the router have a infobase that hold the states
	these infobases also have a readhandler that displays the states in a clean way

	read router/infoBase.read
	  Displays each subnetwork and the reception state for each group on this subnetwork


	read clientxy/infoBase.read
	  Displays which groups this client is a member of
