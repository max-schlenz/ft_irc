# include "Server.hpp"
# include "Client.hpp"
# include "irc.hpp"
# include <fcntl.h>

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

	pollfd server_poll_fd;
    server_poll_fd.fd = this->_sock;
    server_poll_fd.events = POLLIN;
    this->_pollFds.push_back(server_poll_fd);
	this->setCommands();
	std::cout << "Server listening on: " << BWHITE << inet_ntoa(this->_saddr_in.sin_addr) << ":" << this->_port << RESET <<  std::endl;
}

void Server::setCommands()
{
	this->_commands["JOIN"] = &join;
	this->_commands["LEAVE"] = &leave;
	this->_commands["QUIT"] = &quit;
	this->_commands["MSG"] = &msg;
	this->_commands["NICK"] = &nick;
	this->_commands["TOPIC"] = &topic;
	this->_commands["MODE"] = &mode;
	this->_commands["KICK"] = &kick;
	this->_commands["INVITE"] = &invite;
}

void Server::accept_client(std::vector<pollfd>& poll_fds)
{
	sockaddr_in	sin;
	char*		ipStr;
	pollfd		client_poll_fd;
	socklen_t size = sizeof(sin);
	int sock = accept(this->_sock, (struct sockaddr*)&sin, &size);
	if (sock > 0)
	{
		getsockname(sock, (struct sockaddr*)&sin, &size);
		ipStr = inet_ntoa(sin.sin_addr);
		Client client(sin, size, sock, ipStr);

		this->_clients.push_back(client);
		client_poll_fd.fd = client.sock();
		client_poll_fd.events = POLLIN;
		poll_fds.push_back(client_poll_fd);
		
		std::cout << GREEN << "Client " << BGREEN << client.ipStr() << GREEN << " connected." << RESET << std::endl;
	}
}

void Server::startServer()
{
	int res = 0;

	while (true)
	{
		try
		{
			res = poll(this->_pollFds.data(), this->_pollFds.size(), 1000);
			if (res == -1)
				throw Server::ErrorPoll();

		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	
		for (int i = 0; res > 0 && i < this->_pollFds.size(); i++)
		{
			if (this->_pollFds[i].fd == this->_sock && this->_pollFds[i].revents & POLLIN)
				this->accept_client(this->_pollFds);
			else if (this->_pollFds[i].revents & POLLIN)
				this->handleClientReq(i);
		}
	}
}

void Server::handleReqPing(int i, std::string request)
{
	std::string response = "PONG " + request.substr(5);
	// std::cout << GRAY << " << rec: " << request << "; resp: " << response  << RESET << std::endl;
	send(this->_clients[i - 1].sock(), response.c_str(), response.size(), 0);
}

void Server::handleReqHandshake(int i, std::string request)
{
	std::string response = ":127.0.0.1 001 mschlenz :Welcome to The Internet Relay Network mschlenz!mschlenz@mschlenz\r\n";
	// std::cout << GRAY << " << rec: " << request << "; resp: " << response  << RESET << std::endl;
	send(this->_clients[i - 1].sock(), response.c_str(), response.size(), 0);
}

void Server::parseReq(std::string request, int i)
{
	if (request.find("CAP LS") != std::string::npos)
		this->handleReqHandshake(i, request);
	else if (request.find("PING") != std::string::npos)
		this->handleReqPing(i, request);
	else if (request.find("PONG") != std::string::npos)
		std::cout << GRAY << "pong received" << RESET << std::endl;
	else if (request.find("JOIN") != std::string::npos)
		this->exec_cmd("JOIN", "TEST");
	else
		std::cout << "not recognized: " << request << std::flush;
}

void Server::handleClientReq(int i)
{
	char	buffer_arr[RECV_BUF];
	memset(buffer_arr, 0, RECV_BUF);
	int		recv_len = 0;
	
	recv_len = recv(this->_pollFds[i].fd, &buffer_arr, RECV_BUF, 0);
	if (recv_len <= 0)
	{
		std::cout << RED << "Client " << BRED << this->client(i - 1).ipStr() << RED << " disconnected." << RESET << std::endl;
		close(this->_pollFds[i].fd);
		this->_pollFds.erase(this->_pollFds.begin() + i);
		this->_clients.erase(this->_clients.begin() + (i - 1));
	}
	else
	{
		std::string buffer_str = buffer_arr;
		size_t		buffer_str_nl;
		bool		test = false;

		if ((buffer_str_nl = buffer_str.find('\n')) != std::string::npos)
		{
			this->_clients[i - 1].getCmdQueue().push_back(buffer_str.substr(0, buffer_str_nl + 1));
			test = true;
		}
		else
			this->_clients[i - 1].getCmdQueue().push_back(buffer_str);

		if (strchr(buffer_arr, '\n'))
		{
			std::string command;
			for (std::vector<std::string>::iterator it = this->_clients[i - 1].getCmdQueue().begin(); it != this->_clients[i - 1].getCmdQueue().end(); ++it)
				command += *it;
			this->parseReq(command, i);
			this->_clients[i - 1].getCmdQueue().clear();
		}
		if (test)
		{
			this->_clients[i - 1].getCmdQueue().push_back(buffer_str.substr(buffer_str_nl + 1));
		}
		memset(buffer_arr, 0, RECV_BUF);
	}
}

const char* Server::InvalidArgsException::what() const throw()
{
	return "Invalid Arguments";
}

const char* Server::ErrorEstablishingConException::what() const throw()
{
	return "Error establishing connection...";
}

const char* Server::ErrorBindingSocketException::what() const throw()
{
	return "Error binding socket...";
}

const char* Server::ErrorOnAcceptException::what() const throw()
{
	return "Error on accepting...";
}

const char* Server::ErrorGettingProtonException::what() const throw()
{
	return "Error getting proton...";
}

const char* Server::ErrorPoll::what() const throw()
{
	return "Error: poll returned -1";
}
