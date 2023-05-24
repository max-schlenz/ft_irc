#include "Server.hpp"

void Server::pass(std::vector<std::string> reqVec, Client &client)
{
	if (this->_key_set == false)
		std::cout << YELLOW << "No password needed!" << RESET << std::endl;
	else if (reqVec.size() > 1)
	{
		std::string pass = reqVec[1].replace(0,1,"");
		if (reqVec[1] == this->_key)
			std::cout << YELLOW << "Password correct!" << RESET << std::endl;
		else
		{
			std::cout << RED << "Bad password attempt!" << RESET << std::endl;
			client.setKick(true);
		}
	}
}
