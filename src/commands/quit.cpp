#include "Server.hpp"

//":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 QUIT :" + quitMsg + "\r\n";
void Server::quit(std::vector<std::string> reqVec, Client& client)
{
	std::string quitMessage;
	if (reqVec.size() > 1)
	{
		for (std::vector<std::string>::iterator it = reqVec.begin(); it != reqVec.end(); ++it)
		{
			quitMessage += *it;
			if (it + 1 != reqVec.end())
				quitMessage += " ";
		}
	}
	this->sendMsgToAll(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 QUIT :" + quitMessage + "\r\n");
}
