#include "Server.hpp"

void Server::topic(std::vector<std::string> reqVec, Client &client)
{
	if (checkTopic(reqVec, client))
	{
		std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);

		if (itChannel != this->_channelsM.end())
		{
			std::string topic;
			for (std::vector<std::string>::iterator request = reqVec.begin() + 2; request != reqVec.end(); ++request)
			{
				if (*request == reqVec[2] && (*request)[0] == ':')
					(*request).replace(0,1,"");
				topic += *request;
				if (request + 1 != reqVec.end())
					topic += " ";
			}
			topic += "\r\n";
			itChannel->second.setTopic(topic);
			this->sendMsgToAllInChannel(itChannel->second, TOPIC(client, itChannel->second.getName(), topic), "");
		}
		else
			this->sendResponse(client, E_NOSUCHCHANNEL(client, reqVec[1]));
	}
}