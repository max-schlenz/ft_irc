# include "Server.hpp"
# include "Channel.hpp"

void Server::bot(std::vector<std::string> reqVec, Client &client)
{
	std::string response;

	if (reqVec.size() == 1)
	{
		if (this->_bot_on)
			response = NOTICE("Bot is activated!\r\n");
		else
			response = NOTICE("Bot is deactivated!\r\n");
		this->sendResponse(client, response);
	}
	else if (reqVec.size() >= 2)
	{
		if (!this->_bot_on && toUpperCase(reqVec[1]) == "ON")
		{
			this->_bot = new Bot(this->_port);
			response = "NICK Bot\r\n";
			send(this->_bot->getSock(),response.c_str(), response.size(), 0);
			response = "USER Bot 0 * :bot\r\n";
			send(this->_bot->getSock(),response.c_str(), response.size(), 0);
			std::map<std::string, Channel>::iterator it;
			for (it = _channelsM.begin(); it != _channelsM.end(); ++it)
			{
				response = "JOIN " + it->first + "\r\n";
				this->sendResponse(*this->_bot, response);
			}
			this->_bot_on = true;
		}
		else
		{
			std::map<std::string, Channel>::iterator itChannel = this->_channelsM.find(reqVec[1]);
			if (itChannel != this->_channelsM.end())
			{
				response = "JOIN " + reqVec[1] + "\r\n";
				this->sendResponse(*this->_bot, response);
			}
		}
	}
	
}
