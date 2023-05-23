#include "Server.hpp"

// 311 -> found
// 401 -> not found
void Server::whois(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 1 && this->isValidClient(reqVec[1]))
	{
		Client target = this->getClientName(reqVec[1]);
		this->sendResponse(client, ":127.0.0.1 311 " + client.getNickname() + " " + reqVec[1] + " " + target.getNickname() + " " + target.getUsername() + " * :" + target.getRealName() + "\r\n");
	}
	else if (reqVec.size() > 1)
		this->sendResponse(client, ":127.0.0.1 401 " + client.getNickname() + " " + reqVec[1] + " :No such nick/channel\r\n");
	else
		this->sendResponse(client, ":127.0.0.1 311 " + client.getNickname() + " " + reqVec[1] + client.getNickname() + " " + client.getUsername() + " * :" + client.getRealName() + "\r\n");
}