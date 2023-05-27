#include "Server.hpp"

// USER max 0 127.0.0.1 :1,11KVIrc 5.0.0 Aria http://kvirc.net/
void Server::user(std::vector<std::string> reqVec, Client &client)
{
	if (client.getKick())
		return;
	if (this->checkUser(reqVec, client))
	{
		client.setUsername(reqVec[1]);

		std::string realName = (*(reqVec.begin() + 4)).substr(1);
		for (std::vector<std::string>::iterator it = reqVec.begin() + 5; it != reqVec.end(); ++it)
			realName += *it;

		client.setRealname(realName);
	}
	if (!client.getUserRegistered())
	{
		std::string response = ":127.0.0.1 NOTICE * :*** Checking Ident\r\n";
		response += ":127.0.0.1 NOTICE * :*** Looking up your hostname...\r\n";
		response += ":127.0.0.1 NOTICE * :*** No Ident response\r\n";
		response += ":127.0.0.1 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + "127.0.0.1\r\n";
		response += ":127.0.0.1 002 " + client.getNickname() + " :Your host is ft.irc.de, running version 1.0\r\n";
		time_t currentTime = time(NULL);
		char *dateString = ctime(&currentTime);
		response += ":127.0.0.1 003 " + client.getNickname() + " :This server was created " + dateString + "\r\n";
		// response += ":127.0.0.1 004 " + client.getNickname() + " ft.irc.de ft_irc1.0 iw itko\r\n";
		response += ":127.0.0.1 251 " + client.getNickname() + " :There are " + itos(this->_clientsM.size()) + " users and 0 invisible on 1 servers\r\n";
		response += ":127.0.0.1 252 " + client.getNickname() + " " + itos(this->_num_ops) + " :IRC Operators online\r\n";
		response += ":127.0.0.1 253 " + client.getNickname() + " 0 :Unknown connection(s)\r\n";
		response += ":127.0.0.1 254 " + client.getNickname() + " " + itos(this->_num_channels) + " :Channels formed\r\n";
		response += ":127.0.0.1 265 " + client.getNickname() + " " + itos(this->_clientsM.size()) + " " + itos(USR_LIMIT) + " :Current local users " + itos(this->_clientsM.size()) + ", max " + itos(USR_LIMIT) + "\r\n";
		response += ":127.0.0.1 375 " + client.getNickname() + " :- Message of the day\r\n";
		response += ":127.0.0.1 372 " + client.getNickname() + " :- Welcome to our server!\r\n";
		response += ":127.0.0.1 372 " + client.getNickname() + " :- \r\n";
		response += ":127.0.0.1 372 " + client.getNickname() + " :- Have fun :)\r\n";
		response += ":127.0.0.1 376 " + client.getNickname() + " :End of /MOTD command.\r\n";
		response += ":" + client.getNickname() + " MODE " + client.getNickname() + " :+iw\r\n";
		this->sendResponse(client, response);
		client.setNickRegistered(true);
	}
}

