
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "common.hpp"
#include "Client.hpp"

class Client;

class Channel
{
	public:
		const std::string& getTopic() const {
			return (this->_topic);
		}
		const std::string& getPassword() const {
			return (this->_password);
		}
		std::map<std::string, Client*>& getClientsM() {
			return (this->_clientsM);
		}
		void setTopic(std::string topic){
			this->_topic = topic;
		}
		void setPassword(std::string password){
			this->_password = password;
		}
		Channel(std::string topic, std::string password) {
			this->_topic = topic;
			this->_password = password;
		}
		std::string getName() {
			return this->_name;
		}
		void setLimit(unsigned int limit) {
			this->_limit = limit;
		}
		void setModeTrue(char mode) {
			this->_modes[mode] = true;
		}
		void setModeFalse(char mode) {
			this->_modes[mode] = false;
		}
		void addOperator(std::string name, Client* client) {
			this->_operators[name] = client;
		}
		void removeOperator(std::string name) {
			this->_operators.erase(name);
		}
		unsigned int getLimit() {
			return this->_limit;
		}
		std::map<char, bool> getModes() {
			return this->_modes;
		}
		std::map<std::string, Client*>& getOperators() {
			return this->_operators;
		}
		Channel(std::string name);
		Channel();
		
	private:
		std::string	_name;
		std::string	_topic;
		std::string	_password;
		unsigned int	_limit;
		std::map<char, bool>		_modes;
		std::map<std::string, Client*> _operators;
		std::map<std::string, Client*> _clientsM;
};

#endif