
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "common.hpp"
#include "Client.hpp"

class Client;

class Channel
{
	public:
		Channel();
		Channel(std::string name);
		Channel(std::string topic, std::string password);
		
		const std::string& 				getTopic() const;
		const std::string& 				getPassword() const;
		std::map<std::string, Client*>&	getClientsM();
		std::map<std::string, Client*>&	getInvitedClientsM();
		void 							setTopic(std::string topic);
		void 							setPassword(std::string password);
		std::string 					getName();
		void 							setLimit(unsigned int limit);
		void 							setModeTrue(char mode);
		void 							setModeFalse(char mode);
		void 							addOperator(std::string name, Client* client);
		void 							addInvitedClient(std::string name, Client* client);
		void 							removeOperator(std::string name);
		unsigned int 					getLimit();
		std::map<char, bool> 			getModes();
		std::map<std::string, Client*>&	getOperators();

	private:
		std::string						_name;
		std::string						_topic;
		std::string						_password;
		unsigned int					_limit;
		std::map<char, bool>			_modes;
		std::map<std::string, Client*> _operators;
		std::map<std::string, Client*> _invitedClientsM;
		std::map<std::string, Client*> _clientsM;
};

#endif