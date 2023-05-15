#include "Server.hpp"

void Server::part(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		for (std::vector<Channel*>::iterator it = client.getJoinedChannels().begin(); it != client.getJoinedChannels().end(); ++it)	//iterating through Clients joined channel list
		{
			if ((*it)->getName() == reqVec[1])
			{
				for (std::vector<Client*>::iterator it2 = (*it)->getClients().begin(); it2 != (*it)->getClients().end(); ++it2)	//iterating through joined channels clients, finding "itself" and erase field in vec
				{
					if ((*it2)->getNickname() == client.getNickname())
					{
						(*it)->getClients().erase(it2);
						break ;
					}
				}
				size_t numClients = (*it)->getClients().size(); //store size to catch empty channel (for channel deletion)

				std::cout << GRAY << client.getNickname() << " left channel: " << RESET << (*it)->getName() << std::endl;
				
				it = client.getJoinedChannels().erase(it); //removing channel from clients joined channels
				if (numClients == 0) // if no user left, remove channel
				{
					std::cout << GRAY << "   + removed channel: " << RESET << reqVec[1] << std::endl;
					for (std::vector<Channel>::iterator it2 = this->_channels.begin(); it2 != this->_channels.end();)
					{
						if ((*it2).getName() == reqVec[1])
							it2 = this->_channels.erase(it2);
						else
							++it2;
							
					}
				}
				break ;
			}
		}
	}
}

void Server::join(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		for (std::vector<Channel*>::iterator it = client.getJoinedChannels().begin(); it != client.getJoinedChannels().end(); ++it)
		{
			if ((*it)->getName() == reqVec[1])
				return ;
		}

		for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
		{
			if ((*it).getName() == reqVec[1])
			{
				client.getJoinedChannels().push_back(&(*it));
				(*it).getClients().push_back(&client);
				std::cout << GRAY << client.getNickname() << " joined channel: " << RESET << it->getName() << std::endl;
				return ;
			}
		}
		Channel channel(reqVec[1]);
		this->_channels.push_back(channel);

		Channel& channel_ref = this->_channels.back();

		channel_ref.getClients().push_back(&client);
		std::cout << GRAY << client.getNickname() << " created channel: " << RESET << channel.getName() << std::endl;
		client.getJoinedChannels().push_back(&channel_ref);
	}
}

void Server::leave(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "leave" << std::endl;
}

void Server::nick(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		client.setNickname(reqVec[1]);
		std::cout << GRAY << "NICK set to " << client.getNickname() << std::endl;
	}
}

void Server::msg(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "msg" << std::endl;
}

void Server::topic(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "topic" << std::endl;
}

void Server::mode(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "mode" << std::endl;
}

void Server::kick(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "kick" << std::endl;
}

void Server::invite(std::vector<std::string> reqVec, Client& client)
{
	std::cout << "invite" << std::endl;
}

void Server::user(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		client.setUsername(reqVec[1]);
		std::cout << GRAY << "USER set to " << client.getUsername() << std::endl;
	}
}

void Server::ping(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() > 1)
	{
		std::string response = "PONG " + reqVec[1] + "\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
		std::cout << GRAY << "PING recieved from " << client.getIpStr() << " (" << client.getNickname() << ")" << RESET << std::endl;
	}
}
