#include "Server.hpp"

// USER max 0 127.0.0.1 :1,11KVIrc 5.0.0 Aria http://kvirc.net/
void Server::user(std::vector<std::string> reqVec, Client &client)
{
	if (this->checkUser(reqVec, client))
	{
		client.setUsername(reqVec[1]);

		std::string realName = (*(reqVec.begin() + 4)).substr(1);
		for (std::vector<std::string>::iterator it = reqVec.begin() + 5; it != reqVec.end(); ++it)
			realName += *it;

		client.setRealname(realName);
	}
}
