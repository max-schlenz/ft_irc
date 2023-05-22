#include "Server.hpp"

// USER max 0 127.0.0.1 :1,11KVIrc 5.0.0 Aria http://kvirc.net/
/*
recVec[0] == USER
recVec[1] == max
recVec[2] == 0
recVec[3] == 127.0.0.1
recVec[4] == :1,11KVIrc
recVec[5] == 5.0.0
recVec[6] == Aria
recVec[7] == http://kvirc.net/
*/
void Server::user(std::vector<std::string> reqVec, Client &client)
{
	if (checkUser(reqVec, client))
	{
		std::cout << client.getNickname() << GRAY << " USER set to " << reqVec[1] << RESET << std::endl;
		client.setUsername(reqVec[1]);

		std::string realName = (*(reqVec.begin() + 4)).substr(1);
		for (std::vector<std::string>::iterator it = reqVec.begin() + 5; it != reqVec.end(); ++it)
			realName += *it;

		client.setRealname(realName);
	}
}
