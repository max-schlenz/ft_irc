# include "Server.hpp"

static std::string convertIp(Client& client)
{
	uint32_t ipRecDec = htonl(inet_addr(client.getHostname().c_str()));

	std::ostringstream oss;
	oss << ipRecDec;

	return oss.str();
}

//PRIVMSG Receiver :\x01DCC SEND file.txt 3232235776 12345 128\x01
//PRIVMSG User1 :DCC SEND file 2130706433 60052 4
void Server::dcc(std::vector<std::string> reqVec, Client &client)
{
	std::map<std::string, Client*>::iterator itClient = this->_clientsM.find(reqVec[1]);

	if (itClient != this->_clientsM.end())
		this->sendResponse(client, ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 PRIVMSG " + reqVec[1] + " :\1DCC SEND " + reqVec[4] + " " + convertIp(client) + " " + reqVec[6] + " " + reqVec[7] + "\r\n");
}
