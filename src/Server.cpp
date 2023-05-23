# include "Server.hpp"

extern bool g_run;

Server::Server(int port)
{
	// this->_channels.reserve(1024);
	this->_clients.reserve(1024);
	this->_pollFds.reserve(1024);


	this->_proto = getprotobyname("tcp");
	this->_sock = socket(AF_INET, SOCK_STREAM, this->_proto->p_proto);
	this->_port = port;
	this->_saddr_in.sin_family = AF_INET;
	this->_saddr_in.sin_port = htons(this->_port);
	this->_saddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_saddr_in_len = sizeof(this->_saddr_in);
	this->_hostname = inet_ntoa(this->_saddr_in.sin_addr);

	int optval = 1;
    setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	fcntl(this->_sock, F_SETFL, O_NONBLOCK);
	if ((bind(this->_sock, (struct sockaddr*)&this->_saddr_in, this->_saddr_in_len)) < 0)
		error_handling("Error: bind failed!");
	if ((listen(this->_sock, USR_LIMIT)) < 0)
		error_handling("Error: listen failed!");
	pollfd server_poll_fd;
	server_poll_fd.fd = this->_sock;
	server_poll_fd.events = POLLIN;
	this->_pollFds.push_back(server_poll_fd);
	this->setCommands();
}

Server::~Server()
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		close((*it).getSock());
	close(this->_sock);
}

void Server::setCommands()
{
	this->_commands["JOIN"] = &Server::join;
	this->_commands["PART"] = &Server::part;
	this->_commands["LEAVE"] = &Server::leave;
	this->_commands["NICK"] = &Server::nick;
	this->_commands["TOPIC"] = &Server::topic;
	this->_commands["MODE"] = &Server::mode;
	this->_commands["KICK"] = &Server::kick;
	this->_commands["invite"] = &Server::invite;   // lets pleaase make a to lower in the beginning for all ; but lc /join shouldn't work :D 
	this->_commands["INVITE"] = &Server::invite;   // how about this? :) 
	this->_commands["USER"] = &Server::user;
	this->_commands["PING"] = &Server::ping;
	this->_commands["WHOIS"] = &Server::whois;
	this->_commands["CAP"] = &Server::capreq;
	this->_commands["PRIVMSG"] = &Server::privmsg;
	this->_commands["MSG"] = &Server::privmsg; // same as privmsg
	this->_commands["WHO"] = &Server::who;
	this->_commands["NOTICE"] = &Server::notice;

	this->_commands["dbg"] = &Server::dbgPrint;
	this->_commands["dcc"] = &Server::dcc;

	// this->_commands["pac"] = &Server::dbgPrintAllChannels;
}

bool Server::isUserInChannel(Client &client, std::string channelName)
{
	if (client.getJoinedChannels().find(channelName) != client.getJoinedChannels().end())
		return true;
	return false;
}

void Server::sendMsgToAll(Client &client, std::string message)
{
	for (std::map<std::string, Client*>::iterator it = this->_clientsM.begin(); it != this->_clientsM.end(); ++it)
	{
		if (it->first != client.getNickname())
			send(it->second->getSock(), message.c_str(), message.size(), 0);
	}
}

void Server::sendResponse(Client &client, const std::string& response)
{
	send(client.getSock(), response.c_str(), response.size(), 0);
}

bool Server::parseReq(Client& client, std::string request)
{
	std::vector<std::string> reqVec;
	std::string reqField;

	std::istringstream iss(request);
	while (iss >> reqField)
		reqVec.push_back(reqField);

	if (reqVec.size())
	{
		std::map<std::string, void(Server::*)(std::vector<std::string> reqVec, Client& client)>::iterator it = this->_commands.find(reqVec[0]);
		// if (request.find("CAP LS") != std::string::npos)
		// 	this->handleReqHandshake(client, reqVec);
		
		if (it != this->_commands.end())
			(this->*(it->second))(reqVec, client);

		else if (reqVec[0] == "QUIT")
			return false;

		else
			std::cout << GRAY << "not recognized: " RESET << request << std::endl;
	}
	return true;
}

bool Server::isValidClient(std::string name)
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->getNickname() == name)
			return true;
	}
	return false;
}

// ! use isValidClient() as protection !
Client &Server::getClientName(std::string name)
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->getNickname() == name)
			return (*it);
	}
	Client *err;
	return *(err);
}

bool Server::parseReqQueue(Client& client)
{
	for (std::vector<std::string>::iterator it = client.getReqQueue().begin(); it != client.getReqQueue().end(); ++it)
	{
		if (!this->parseReq(client, *it))
			return false;
	}
	client.getReqQueue().clear();
	return true;
}

// void Server::buildReqQueue(Client& client, std::vector<char> buf)
// void Server::buildReqQueue(Client& client, std::vector<char>& buf)
void Server::buildReqQueue(Client& client, const std::string& buffer)
{
	std::istringstream iss(buffer);
	std::string buffer_str;

	while (std::getline(iss, buffer_str, '\n'))
		client.getReqQueue().push_back(buffer_str);
}

bool Server::handleClientReq(Client& client)
{
	std::vector<char> buffer(RECV_BUF);
	std::fill(buffer.begin(), buffer.end(), '\0');

	int recv_len = recv(client.getPollFd().fd, buffer.data(), RECV_BUF, 0);

	if (recv_len <= 0)
		return false;

	else
	{		
		std::string buf(buffer.begin(), buffer.begin() + recv_len);
		
		std::cout << BLUE << " < " << buf << RESET << std::flush;
		this->buildReqQueue(client, buf);
		if (!this->parseReqQueue(client))
			return false;
	}
	return true;
}

void Server::acceptClient()
{
	sockaddr_in sin;
	socklen_t size = sizeof(sin);
	int sock = accept(this->_sock, (struct sockaddr*)&sin, &size);
	if (sock > 0)
	{
		getsockname(sock, (struct sockaddr*)&sin, &size);
		std::string ipStr = inet_ntoa(sin.sin_addr);

		pollfd client_poll_fd;
		client_poll_fd.fd = sock;
		client_poll_fd.events = POLLIN;
		this->_pollFds.push_back(client_poll_fd);

		Client client(sin, sock, ipStr, client_poll_fd);
		this->_clients.push_back(client);
	}
}

void Server::disconnectClient(Client& client, int i)
{
	std::cout << RED << "Client " << BRED << this->_clients[i - 1].getHostname() << RED << " disconnected." << RESET << std::endl;
	close(this->_pollFds[i].fd);
	this->_clientsM.erase(this->_clients[i - 1].getNickname());
	this->_pollFds.erase(this->_pollFds.begin() + i);
	this->_clients.erase(this->_clients.begin() + (i - 1));
}

void Server::startServer()
{
	int res = 0;

	if (g_run)
		std::cout << "Server listening on: " << BWHITE << inet_ntoa(this->_saddr_in.sin_addr) << ":" << this->_port << RESET <<  std::endl;
	while (g_run)
	{
		res = poll(this->_pollFds.data(), this->_pollFds.size(), 500);
		for (int i = 0; res > 0 && i < this->_pollFds.size(); i++)
		{
			if (this->_pollFds[i].fd == this->_sock && this->_pollFds[i].revents & POLLIN && this->_clients.size() < USR_LIMIT)
				this->acceptClient();

			else if (i > 0 && this->_pollFds[i].revents & POLLIN)
			{
				if (!this->handleClientReq(this->_clients[i - 1]))
					this->disconnectClient(this->_clients[i - 1], i);
			}
		}
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
