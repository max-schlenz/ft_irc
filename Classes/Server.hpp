#ifndef SERVER_HPP
# define SERVER_HPP
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <string.h>
# include <string>
# include <netinet/in.h>
 #include "Client.hpp"
 #include <vector>

// typedef struct sockaddr_in sockaddr_in;
// typedef struct protoent protoent;

class Server {
	public:
		void accept_client();
		void set_sock(int sock) {
			this->_sock = sock;
		};
		void set_port(int port) {
			this->_port = port;
		};
		void set_saddr_in(sockaddr_in saddr_in) {
			this->_saddr_in_len = sizeof(saddr_in);
			this->_saddr_in = saddr_in;
		};
		void set_sinLen(socklen_t saddr_in_len) {
			this->_saddr_in_len = saddr_in_len;
		}
		const int& sock() const {
			return (this->_sock);
		}
		const int& port() const {
			return (this->_port);
		}
		sockaddr_in& saddr_in() {
			return (this->_saddr_in);
		}
		Client& client() {
			return (this->_clients.front());
		}
		socklen_t& saddr_in_len() {
			return (this->_saddr_in_len);
		}
		Server(){};
		Server(int port, int sock, sockaddr_in saddr_in) : _sock(sock), _port(port), _saddr_in(saddr_in){};
		~Server(){};
	private:
		int	_sock;
		int _port;
		int _client;
		sockaddr_in _saddr_in;
		socklen_t _saddr_in_len;
		std::vector<Client> _clients;
};

#endif