# include "Server.hpp"

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
		
		if (it != this->_commands.end())
			(this->*(it->second))(reqVec, client);

		else if (reqVec[0] == "QUIT")
			return false;

		else
			std::cout << GRAY << "not recognized: " RESET << request << std::endl;
	}
	return true;
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
		
		std::cout << BLUE << buf << RESET << std::flush;
		this->buildReqQueue(client, buf);
		if (!this->parseReqQueue(client))
			return false;
	}
	return true;
}