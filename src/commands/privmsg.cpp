#include "Server.hpp"

// static int getFileSize(char* filename)
// {
// 	int fd = -1;

// 	fcntl(f, F_DUPFD)
// }

//: NICK!USER@host PRIVMSG <recipient> :<text>
// PRIVMSG <recipient>{,<recipient>} <text to be sent>
//
//PRIVMSG User1 :DCC SEND file 2130706433 60052 4
void Server::privmsg(std::vector<std::string> reqVec, Client &client)
{
	if (reqVec.size() > 2)
	{
		if (reqVec[2] == ":\1DCC")
			std::cout << BRED << reqVec[2] << RESET << std::endl;
		if (reqVec[2] == ":\1DCC" && reqVec.size() > 3 && reqVec[3] == "SEND")
		{
			PRNT;
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
				return;
			}
		}
		else
		{
			for (std::map<std::string, Client *>::iterator itClient = this->_clientsM.begin(); itClient != this->_clientsM.end(); ++itClient)
			{
				if (itClient->first != client.getNickname() && isUserInChannel(*itClient->second, reqVec[1]))
				{
					std::string response = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 PRIVMSG " + reqVec[1] + " :";
					if (reqVec[2].length() > 1)
						reqVec[2] = reqVec[2].substr(1);
					for (std::vector<std::string>::iterator itVec = reqVec.begin() + 2; itVec != reqVec.end(); ++itVec)
					{
						response += *itVec;
						if (itVec + 1 != reqVec.end())
							response += " ";
					}
					response += "\r\n";
					send(itClient->second->getSock(), response.c_str(), response.size(), 0);
				}
			}
		}
	}
}
