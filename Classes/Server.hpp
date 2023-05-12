
#ifndef SERVER_HPP
# define SERVER_HPP

# define RECV_BUF 1024

# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <string.h>
# include <string>
# include <netinet/in.h>
# include "Client.hpp"
# include "irc.hpp"
# include <vector>
# include <poll.h>
# include <stdexcept>

typedef struct sockaddr_in sockaddr_in;
typedef struct protoent protoent;

class Server {
	public:
		class	InvalidArgsException: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		
		class	ErrorEstablishingConException: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	ErrorBindingSocketException: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		
		class	ErrorOnAcceptException: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		class	ErrorGettingProtonException: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		class	ErrorPoll: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		void accept_client(std::vector<pollfd>& poll_fds);
		void set_sock(int sock) {
			this->_sock = sock;
		};
		void set_port(int port) {
			this->_port = port;
		};
		void set_sin(sockaddr_in _saddr_in) {
			this->_saddr_in_len = sizeof(_saddr_in);
			this->_saddr_in = _saddr_in;
		};
		void set_sinLen(socklen_t _saddr_in_len) {
			this->_saddr_in_len = _saddr_in_len;
		}
		const int& sock() const {
			return (this->_sock);
		}
		const int& port() const {
			return (this->_port);
		}
		sockaddr_in& sin() {
			return (this->_saddr_in);
		}
		Client& client(int i) {
			return (this->_clients[i]);
		}
		socklen_t& sinLen() {
			return (this->_saddr_in_len);
		}
		// Server(int port, std::vector<pollfd>& poll_fds);
		Server(int port);
		Server(int port, int sock, sockaddr_in _saddr_in) : _sock(sock), _port(port), _saddr_in(_saddr_in){};
		~Server(){};
		void startServer();
		void handleReqPing(int i, std::string request);
		void handleClientReq(int i);
		void handleReqHandshake(int i, std::string command);
		void parseReq(std::string command, int i);

	private:
		int	_sock;
		int _port;
		protoent* _proto;
		int _client;
		sockaddr_in _saddr_in;
		socklen_t _saddr_in_len;
		
		std::vector<Client> _clients;
		std::vector<pollfd> _pollFds;
};

#endif