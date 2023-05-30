# include "Server.hpp"

void Server::bot(std::vector<std::string> reqVec, Client &client)
{
	std::string response;

	if (reqVec.size() >= 2)
	{
		if (!this->_bot_on && toUpperCase(reqVec[1]) == "ON")
		{
			this->_bot = new Bot(this->_port, this->_hostname);
			response = "CAP LS\r\nPING :127.0.0.1\r\n";
			send(this->_bot->getSock(),response.c_str(), response.size(), 0);
			this->_bot_on = true;
		}
		else if (toUpperCase(reqVec[1]) == "CAP")
		{
			response = "CAP END\r\nNICK bot\r\nUSER bot 0 127.0.0.1 :1,0 1.0.0 Den http://google.com\r\n";
			send(this->_bot->getSock(),response.c_str(), response.size(), 0);
		}
		else
		{
			std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);
			if (itChannel != this->_channelsM.end())
			{
				response = JOIN((*this->_bot), reqVec[1]);
				this->sendResponse(*this->_bot, response);
			}

		}
	}
}
