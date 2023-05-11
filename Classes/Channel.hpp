#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "Client.hpp"
# include <array>

class Channel
{
	public:
		const std::string& get_topic() const {
			return (this->topic);
		}
		const std::string& get_password() const {
			return (this->password);
		}
		const int get_num_clients() const {
			return (this->num_clients);
		}
		const int get_channel_size() const {
			return (this->channel_size);
		}
		std::vector<Client>& get_clients() {
			return (this->_clients);
		}
		void set_topic(std::string topic){
			this->topic = topic;
		}
		void set_password(std::string password){
			this->password = password;
		}
		void set_channel_size(int size) {
			this->channel_size = size;
		}
		void add_client(Client client){
			this->_clients.push_back(client);
		}
		Channel(std::string topic, std::string password, int size) : num_clients(0) {
			this->topic = topic;
			this->password = password;
			this->channel_size = size;
		}
		Channel(){}
		~Channel(){}
	private:
		std::vector<Client> _clients;
		std::string	topic;
		std::string	password;
		std::string	modes;
		bool		mode_i;
		bool		mode_t;
		bool		mode_k;
		bool		mode_o;
		int			num_clients;
		int			channel_size;
};

#endif