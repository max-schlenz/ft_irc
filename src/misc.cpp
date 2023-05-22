#include "Server.hpp"

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
