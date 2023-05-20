
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include "irc.hpp"
#include "Client.hpp"
#include <iostream>

class Channel
{
	public:
		const std::string& getTopic() const {
			return (this->_topic);
		}
		const std::string& getPassword() const {
			return (this->_password);
		}
		std::vector<Client*>& getClients() {
			return (this->_clients);
		}
		void setTopic(std::string topic){
			this->_topic = topic;
		}
		void setPassword(std::string password){
			this->_password = password;
		}
		// void add_client(Client client){
		// 	this->_clients.push_back(client);
		// }
		Channel(std::string topic, std::string password){
			this->_topic = topic;
			this->_password = password;
		}
		std::string getName() {
			return this->_name;
		}
		void setLimit(int limit) {
			this->_limit = limit;
		}
		int getLimit() {
			return this->_limit;
		}

		Channel(std::string name);
		~Channel();

	private:
		std::string	_name;
		std::string	_topic;
		std::string	_password;
		std::string	_modes;
		int			_limit;
		bool		mode_i;
		bool		mode_t;
		bool		mode_k;
		bool		mode_o;

		std::vector<Client*> _clients;
};

#endif