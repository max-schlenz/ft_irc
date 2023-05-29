#include "Server.hpp"

void Server::ping(std::vector<std::string> reqVec, Client &client)
{
	std::string pingMessage;
	if (reqVec.size() > 1)
	{
		for (std::vector<std::string>::iterator itVec = reqVec.begin() + 1; itVec != reqVec.end(); ++itVec)
		{
			pingMessage += *itVec;
			if (itVec + 1 != reqVec.end())
				pingMessage += " ";
		}
		pingMessage += "\r\n";
	}
	this->sendResponse(client, "PONG " + pingMessage + "\r\n");
}
