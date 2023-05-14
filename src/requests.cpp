# include "Server.hpp"

void Server::handleReqHandshake(Client& client, std::vector<std::string> reqVec)
{
	std::string response = ":127.0.0.1 001 tdehne :Welcome to the Internet Relay Network tdehne!tdehne@tdehne\r\n";
	send(client.getSock(), response.c_str(), response.size(), 0);
}
