#include "Server.hpp"

void Server::partAllChannels(Client& client)
{
	if (client.getJoinedChannels().size())
	{
		std::string partChannels;
		for (std::map<std::string, Channel*>::iterator it = client.getJoinedChannels().begin(); it != client.getJoinedChannels().end(); ++it)
			partChannels += it->first + ",";
		
		std::vector<std::string> reqVec;
		reqVec.push_back("PART");
		reqVec.push_back(partChannels.substr(0, partChannels.size() - 1));
		this->part(reqVec, client);
	}
}

void Server::quit(std::vector<std::string> reqVec, Client& client)
{
	this->partAllChannels(client);
	std::string message;
	if (reqVec.size() > 1)
	{
		for (std::vector<std::string>::iterator it = reqVec.begin(); it != reqVec.end(); ++it)
		{
			message += *it;
			if (it + 1 != reqVec.end())
				message += " ";
			else
				message += "\r\n";
		}
	}
	this->sendResponse(client, E_QUIT(client, message));
}
