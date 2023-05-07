#include "Server.hpp"
#include "irc.hpp"

void init_server(Server& server, int port)
{
	protoent *proto = getprotobyname("tcp");
	sockaddr_in sin;
	int			sock;

	if (!proto)
		exiting(4);
	server.set_sock(socket(AF_INET, SOCK_STREAM, proto->p_proto));
	if (sock < 0)
		exiting(1);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	server.set_sin(sin);
	if (bind(server.sock(), (struct sockaddr*)&server.sin(), server.sinLen()) < 0)
		exiting(2);
	listen(server.sock(), 1);
}
