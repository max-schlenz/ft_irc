# include <Server.hpp>
# include <Client.hpp>

void	exiting(int error_code);

void Server::accept_client()
{
	Client client = Client();
	socklen_t size = sizeof(client.sin());
	int id = accept(this->_sock, (struct sockaddr*)&client.sin(), &size);
	client.set_id(id);
	if (client.id() > 0) {
		// exiting(3);
		client.set_sinLen(sizeof(this->_sin));
		getsockname(client.id(), (struct sockaddr*)&client.sin(), &client.sinLen());
		client.set_ipstr();
		std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
		this->_clients.push_back(client);
	}
}

Server::Server()
{
	this->_proto = getprotobyname("tcp");
	this->_sock = socket(AF_INET, SOCK_STREAM, this->_proto->p_proto);
	this->_port = 6667;
	this->_sin.sin_family = AF_INET;
	this->_sin.sin_port = htons(this->_port);
	this->_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_sinLen = sizeof(this->_sin);

	int optval = 1;
    setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	fcntl(this->_sock, F_SETFL, O_NONBLOCK);
	bind(this->_sock, (struct sockaddr*)&this->_sin, this->_sinLen);
	listen(this->_sock, 5);
}
