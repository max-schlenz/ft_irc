#include "Server.hpp"
# include <cstdlib>
# include <sys/wait.h>

void Server::nick(std::vector<std::string> reqVec, Client &client)
{

	if (client.getKick())
		return;
	if (this->checkNick(reqVec, client))
	{
		std::string oldNick = client.getNickname();
		std::string newNick = reqVec[1];
 		/*if (this->_clientsM.size() != 0 && this->_clientsM.find(newNick) != this->_clientsM.end()) {
			this->sendNewNickReq(reqVec, client);
			return;
		}	*/
		client.setNickname(newNick);
		std::string response;
		if (!client.getNickRegistered())
		{
			if (this->_key_set && !client.getPass())
			{
				this->sendResponse(client, E_PASSWDMISMATCH(client));
				this->sendResponse(client, std::string("ERROR No password provided!\r\n"));
				std::cout << RED << "No password provided by client!" << RESET << std::endl;
				client.setKick(true);
				return;
			}
			client.setNickRegistered(true);
			this->_clientsM[client.getNickname()] = &client;
			if (client.getUserRegistered())
				this->welcome(client);
		} else {
			this->sendMsgToAll(client, ":" + oldNick + "!" + client.getUsername() + "@127.0.0.1 NICK " + reqVec[1] + "\r\n");
			for (std::map<std::string, Channel>::iterator it = this->_channelsM.begin(); it != this->_channelsM.end(); ++it)
			{
				if (it->second.getClientsM().find(oldNick) != it->second.getClientsM().end()) {
					it->second.getClientsM().erase(oldNick);
					it->second.getClientsM()[newNick] = &client;
				}
			}
			this->_clientsM.erase(oldNick);
			client.setNickRegistered(true);
			this->_clientsM[newNick] = &client;
		}
	}
}
