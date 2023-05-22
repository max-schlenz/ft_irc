#include "Server.hpp"

//: oldNick!~UserName@host NICK newNick
void Server::nick(std::vector<std::string> reqVec, Client &client)
{
	if (checkNick(reqVec, client))
	{
		std::string oldNick = client.getNickname();
		client.setNickname(reqVec[1]);

		std::string response = ":" + oldNick + "!" + client.getUsername() + "@127.0.0.1 NICK " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);

		std::cout << oldNick << GRAY << " NICK set to " << reqVec[1] << RESET << std::endl;

		if (!client.getRegistered())
		{
			std::string response = ":127.0.0.1 001 " + client.getNickname() + " :welcome, " + client.getNickname() + "!" + client.getUsername() + "@" + "127.0.0.1\r\n";
			send(client.getSock(), response.c_str(), response.size(), 0);
			client.setRegistered(true);
			this->_clientsM[client.getNickname()] = &client;
			PRNT;
		}
	}
}
