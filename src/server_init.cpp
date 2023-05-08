#include "Server.hpp"
#include "irc.hpp"

void init_server(Server& server, int port)
{
	protoent *proto = getprotobyname("tcp");
	sockaddr_in saddr_in;
	int			sock;

	if (!proto)
		exiting(4);
	server.set_sock(socket(AF_INET, SOCK_STREAM, proto->p_proto));
	if (server.sock() < 0)
		exiting(1);
	std::cout << "Socket created!" << std::endl;
	saddr_in.sin_family = AF_INET;
	saddr_in.sin_port = htons(port);
	saddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	server.set_saddr_in(saddr_in);
	server.set_port(port);
	if (bind(server.sock(), (struct sockaddr*)&server.saddr_in(), server.saddr_in_len()) < 0)
		exiting(2);
	listen(server.sock(), 5);
}