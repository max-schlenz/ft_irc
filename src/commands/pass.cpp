#include "Server.hpp"

void Server::pass(std::vector<std::string> reqVec, Client &client)
{
	if (client.getNickRegistered())
		return this->sendResponse(client, E_ALREADYREGISTERED(client));
	client.setPass(true);
	if (this->_key_set == false)
		std::cout << YELLOW << "No password needed!" << RESET << std::endl;
	else if (reqVec.size() > 1)
	{
		std::string pass = reqVec[1].replace(0,1,"");
		if (reqVec[1] == this->_key)
			std::cout << YELLOW << "Password correct!" << RESET << std::endl;
		else
		{
			this->sendResponse(client, E_PASSWDMISMATCH(client));
			this->sendResponse(client, std::string("ERROR Wrong password!\r\n"));
			client.setKick(true);
		}
	}
	else
	{
		this->sendResponse(client, E_NEEDMOREPARAMS(client, "PASS"));
		client.setKick(true);
	}
}
