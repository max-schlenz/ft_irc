# include "Server.hpp"

//PRIVMSG Receiver :\x01DCC SEND file.txt 3232235776 12345 128\x01
//PRIVMSG User1 :DCC SEND file 2130706433 60052 4
void Server::dcc(std::vector<std::string> reqVec, Client &client)
{
	uint32_t ipRecDec = htonl(inet_addr(client.getHostname().c_str()));
	std::map<std::string, Client*>::iterator itClient = this->_clientsM.find(reqVec[1]);

	std::ostringstream oss;
	oss << ipRecDec;
	std::string ipRecDecStr = oss.str();

	if (itClient != this->_clientsM.end())
	{
		std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 PRIVMSG " + reqVec[1] + " :\1DCC SEND " + reqVec[4] + " " + ipRecDecStr + " " + reqVec[6] + " 4\x01\r\n";
		send(itClient->second->getSock(), response.c_str(), response.size(), 0);
		std::cout << BRED << response << RESET << std::endl;
	}
}
