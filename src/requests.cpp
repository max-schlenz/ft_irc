# include "Server.hpp"

void Server::handleReqHandshake(Client& client, std::vector<std::string> reqVec)
{
	std::string response = ":127.0.0.1 001 " + client.getNickname() + " :\002\037\0038Welcome to our dmt.irc server!\002\037\0038\r\n";
	send(client.getSock(), response.c_str(), response.size(), 0);
}
