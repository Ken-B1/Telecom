# Telecom
Implemented:
	- Clients can send join and leave messages
	- Router can process these messages and forward server packets to the correct users

Usage:
	To command the clients, you can use telnet to call handlers

	clients have 2 handlers, a join handler and a leave handler
	
	write clientxy/user.join group [Group address]
	write clientxy/user.leave group [Group address]

	clients and the router have a infobase that hold the states
	these infobases also have a readhandler that displays the states in a clean way

	read router/infoBase.read
	  Displays each subnetwork and the reception state for each group on this subnetwork


	read clientxy/infoBase.read
	  Displays which groups this client is a member of
