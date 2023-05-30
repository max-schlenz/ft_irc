#include "Server.hpp"

std::string itos(const int i)
{
	std::ostringstream oss;
	oss << i;

	return oss.str();
}

void createLst(std::string req, std::vector<std::string>& lst)
{
	std::string buffer;
	std::istringstream iss(req);

	while (getline(iss, buffer, ',')){
		lst.push_back(buffer);
		buffer.clear();
	}
}

std::string toLowerCase(const std::string& str) {
	std::string result = str;
	for (std::size_t i = 0; i < result.length(); ++i) {
		result[i] = std::tolower(result[i]);
	}
	return result;
}

std::string toUpperCase(const std::string& str) {
	std::string result = str;
	for (std::size_t i = 0; i < result.length(); ++i) {
		result[i] = std::toupper(result[i]);
	}
	return result;
}

void Server::sendMsgToAllInChannel(Channel& channel, const std::string& response, const std::string& nickname)
{
	for (std::map<std::string, Client*>::iterator it = channel.getClientsM().begin(); it != channel.getClientsM().end(); ++it)
	{
		if (nickname == "" || it->first != nickname)
			this->sendResponse(*it->second, response);
	}
}

void Server::dbgPrint(std::vector<std::string> reqVec, Client &client)
{
	std::map<std::string, Client*>::iterator clientIt = this->_clientsM.begin();
	std::map<std::string, Channel>::iterator channelIt = this->_channelsM.begin();

	int numClients = 0;
	int numChannels = 0;

	std::cout << BBLUE
	<< "┌────────────┐   ┌────────────┐" << std::endl
	<< "│   USERS    │   │  CHANNELS  │" << std::endl
	<< "├────────────┤   ├────────────┤" << std::endl;

	while (clientIt != this->_clientsM.end() || channelIt != this->_channelsM.end())
	{
		std::cout << "│ " << RESET;
		if (clientIt != this->_clientsM.end())
		{
			std::cout << std::setw(11) << std::left << clientIt->second->getNickname();
			++clientIt;
			++numClients;
		}
		else
			std::cout << std::setw(11) << "";

		std::cout << BBLUE << "│   │ " << RESET;

		if (channelIt != this->_channelsM.end())
		{
			std::cout << std::setw(11) << std::left << channelIt->second.getName();
			++channelIt;
			++numChannels;
		}
		else
			std::cout << std::setw(11) << "";

		std::cout << BBLUE << "│" << std::endl;
	}

	std::cout << BBLUE
	<< "└────────────┘   └────────────┘" << RESET << std::endl;
	std::cout << " " << numClients << " user(s)" << "        " << numChannels << " channel(s)" << std::endl;
}
