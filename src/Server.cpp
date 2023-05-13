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

bool Server::checkCmd(std::vector<std::string> req)
{
	std::map<std::string, void(*)(std::string)>::iterator it = this->_commands.find(req[0]);

	if (it == this->_commands.end()) {
		return (false);
	}
	return (true);
}

void Server::accept_client()
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

		client_poll_fd.fd = sock;
		client_poll_fd.events = POLLIN;
		this->_pollFds.push_back(client_poll_fd);

		// client.setPollFd(client_poll_fd);

		Client client(sin, size, sock, ipStr, client_poll_fd);
		this->_clients.push_back(client);

		// std::cout << client_poll_fd.fd << std::endl;
		// std::cout << (this->_clients.end() - 1)->getPollFd().fd << std::endl;
		
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
				this->accept_client();

			else if (this->_pollFds[i].revents & POLLIN)
			{
				// std::cout << "i: " << i << std::endl;
				// std::cout << "-> " << this-_clients[i - 1].getPollFd().fd << std::endl;
				this->handleClientReq(this->_clients[i - 1], i);
			}
		}
	}
}

bool Server::parseReq(std::string request, int i)
{
	std::vector<std::string> reqVec;
	std::string reqField;

	std::istringstream iss(request);
	while (iss >> reqField)
		reqVec.push_back(reqField);

	if (request.find("CAP LS") != std::string::npos)
		this->handleReqHandshake(i, reqVec);
	
	else if (request.find("PING") != std::string::npos)
		this->handleReqPing(i, reqVec);
		
	else if (request.find("NICK") != std::string::npos)
		this->handleReqNick(i, reqVec);

	else if (request.find("USER") != std::string::npos)
		this->handleReqUser(i, reqVec);
	
	else if (request.find("MODE") != std::string::npos)
		this->handleReqMode(i, reqVec);
	
	else if (request.find("QUIT") != std::string::npos)
	{
		this->handleReqQuit(i);
		return false;
	}

	else
		std::cout << GRAY << "not recognized: " RESET << request << std::endl;
	return true;
}

void Server::handleClientReq(Client& client, int i)
{
	char	buffer_arr[RECV_BUF];
	memset(buffer_arr, 0, RECV_BUF);
	int		recv_len = 0;
	
	// std::cout << client.getPollFd().fd << std::endl;
	// std::cout << this->_clients[i - 1].getPollFd().fd << std::endl;
	// recv_len = recv(this->_pollFds[i].fd, &buffer_arr, RECV_BUF, 0);
	recv_len = recv(client.getPollFd().fd, &buffer_arr, RECV_BUF, 0);

	// std::cout << this->_pollFds[i].fd << std::endl;

	if (recv_len <= 0)
		this->handleReqQuit(i);

	else
	{		
		std::istringstream iss(buffer_arr);
		std::string buffer_str;
		while (std::getline(iss, buffer_str, '\n'))
			client.getCmdQueue().push_back(buffer_str);

		for (std::vector<std::string>::iterator it = client.getCmdQueue().begin(); it != client.getCmdQueue().end(); ++it)
		{
			if (!this->parseReq(*it, i))
				return ;
		}
		client.getCmdQueue().clear();
	
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
