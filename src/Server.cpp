# include "Server.hpp"
# include "Client.hpp"
# include "irc.hpp"
# include <fcntl.h>

void Server::accept_client(std::vector<pollfd>& poll_fds)
{
	sockaddr_in	sin;
	char*		ipStr;
	pollfd		client_poll_fd;
	socklen_t size = sizeof(sin);
	int sock = accept(this->_sock, (struct sockaddr*)&sin, &size);
	if (sock > 0) {
		getsockname(sock, (struct sockaddr*)&sin, &size);
		ipStr = inet_ntoa(sin.sin_addr);
		Client client(sin, size, sock, ipStr);

		this->_clients.push_back(client);
		client_poll_fd.fd = client.id();
		client_poll_fd.events = POLLIN;
		poll_fds.push_back(client_poll_fd);

		std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
	}
}

Server::Server(int port, std::vector<pollfd>& poll_fds)
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

	pollfd server_poll_fd;
	// memset(&server_poll_fd, 0, sizeof(server_poll_fd));
    server_poll_fd.fd = this->_sock;
    server_poll_fd.events = POLLIN;
    poll_fds.push_back(server_poll_fd);
}
