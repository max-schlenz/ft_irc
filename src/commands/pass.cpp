#include "Server.hpp"

void Server::pass(std::vector<std::string> reqVec, Client &client)
{
	if (client.getNickRegistered())
	{
		std::string response = E_ALREADYREGISTERED(client);
		this->sendResponse(client, response);
		return;
	}
	client.setPass(true);
	if (this->_key_set == false)
		std::cout << YELLOW << "No password needed!" << RESET << std::endl;
	else if (reqVec.size() > 1)
	{
		std::string pass = reqVec[1].replace(0,1,"");
		if (reqVec[1] == this->_key)
		{
			std::cout << YELLOW << "Password correct!" << RESET << std::endl;
		}
		else
		{
			std::string response = E_PASSWDMISMATCH(client);
			this->sendResponse(client, response);
			client.setKick(true);
		}
	}
	else
	{
		std::string response = E_NEEDMOREPARAMS(client, "PASS");
		this->sendResponse(client, response);
		client.setKick(true);
	}
}
