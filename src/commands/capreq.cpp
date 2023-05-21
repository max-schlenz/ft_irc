#include "Server.hpp"

void Server::capreq(std::vector<std::string> reqVec, Client &client)
{
	// if (reqVec.size() > 1)
	// {
	std::string response = ":127.0.0.1 CAP * LS :multi-prefix away-notify account-notify\r\n";
	send(client.getSock(), response.c_str(), response.size(), 0);
	// }
	std::cout << GRAY;
	for (std::vector<std::string>::iterator it = reqVec.begin(); it != reqVec.end(); ++it)
		std::cout << *it + " ";
	std::cout << RESET << std::endl;
	
}
