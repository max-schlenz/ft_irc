# include "Server.hpp"
# include "Client.hpp"
# include <fcntl.h>

void	exiting(int error_code);

// void Server::accept_client()
// {
// 	Client client = Client();
// 	socklen_t size = sizeof(client.sin());
// 	int id = accept(this->_sock, (struct sockaddr*)&client.sin(), &size);
// 	client.set_id(id);
// 	if (client.id() > 0) {
// 		// exiting(3);
// 		client.set_sinLen(sizeof(this->_saddr_in));
// 		getsockname(client.id(), (struct sockaddr*)&client.sin(), &client.sinLen());
// 		client.set_ipstr();
// 		std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
// 		this->_clients.push_back(client);
// 	}
// }

void Server::accept_client()
{
	sockaddr_in sin;
	socklen_t size = sizeof(sin);
	char* ipStr;
	int sock = accept(this->_sock, (struct sockaddr*)&sin, &size);
	if (sock > 0) {
		getsockname(sock, (struct sockaddr*)&sin, &size);
		ipStr = inet_ntoa(sin.sin_addr);
		Client client(sin, size, sock, ipStr);
		this->_clients.push_back(client);
		std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
	}
}

Server::Server(int port)
{
	this->_proto = getprotobyname("tcp");
	this->_sock = socket(AF_INET, SOCK_STREAM, this->_proto->p_proto);
	this->_port = port;
	this->_saddr_in.sin_family = AF_INET;
	this->_saddr_in.sin_port = htons(this->_port);
	this->_saddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_saddr_in_len = sizeof(this->_saddr_in);

	int optval = 1;
    setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	fcntl(this->_sock, F_SETFL, O_NONBLOCK);
	bind(this->_sock, (struct sockaddr*)&this->_saddr_in, this->_saddr_in_len);
	listen(this->_sock, 5);
}
