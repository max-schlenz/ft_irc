
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "common.hpp"
// # include "irc.hpp"
// # include "Server.hpp"

#include "Client.hpp"

class Client;
//class Bot;

class Channel
{
	public:
		const std::string& getTopic() const {
			return (this->_topic);
		}
		const std::string& getPassword() const {
			return (this->_password);
		}
		// std::vector<Client*>& getClients() {
		// 	return (this->_clients);
		// }
		std::map<std::string, Client*>& getClientsM() {
			return (this->_clientsM);
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
		// std::vector<std::string>& getClientNames() {
		// 	return this->_clientNames;
		// }
		
		// std::map<std::string, Client*>& getClientMap() {
		// 	return this->_clientMap;
		// }

		Channel(std::string topic, std::string password, int size) : num_clients(0) {
			this->_topic = topic;
			this->_password = password;
		}
		std::string getName() {
			return this->_name;
		}
		void setLimit(int limit) {
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
		int getLimit() {
			return this->_limit;
		}
		std::map<char, bool> getModes() {
			return this->_modes;
		}
		std::map<std::string, Client*>& getOperators() {
			return this->_operators;
		}
		Channel(std::string name);
		// Channel(): _name(""), _topic(""), _password(""), _modes(""), _limit(0), mode_i(false), mode_t(false), mode_k(false), mode_o(false), num_clients(0) {};
		Channel();
		~Channel();

	private:
		std::string	_name;
		std::string	_topic;
		std::string	_password;
		// std::string	_modes;
		int			_limit;
		// bool		mode_i;
		// bool		mode_t;
		// bool		mode_k;
		// bool		mode_o;
		int			num_clients;
		// int			num_clients;

		// std::vector<Client*> _clients;
		// std::vector<std::string> _clientNames;
		// std::vector<std::string> _operators;
		std::map<char, bool>		_modes;
		std::map<std::string, Client*> _operators;
		std::map<std::string, Client*> _clientsM;
};

#endif