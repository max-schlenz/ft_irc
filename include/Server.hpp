
#ifndef SERVER_HPP
# define SERVER_HPP

# define RECV_BUF 1024
# define USR_LIMIT 1000

# include "common.hpp"
# include "irc.hpp"

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
		void setSock(int sock) {
			this->_sock = sock;
		};
		void setPort(int port) {
			this->_port = port;
		};
		void setSin(sockaddr_in _saddr_in) {
			this->_saddr_in_len = sizeof(_saddr_in);
			this->_saddr_in = _saddr_in;
		};
		void setSinLen(socklen_t _saddr_in_len) {
			this->_saddr_in_len = _saddr_in_len;
		}
		void setCommands() ;
		bool isUserInChannel(Client &client, std::string channelName);
		void sendMsgToAll(Client &client, std::string message);
		void sendResponse(Client &client, const std::string &response);
		bool checkCmd(std::vector<std::string> req);
		const int& getSock() const {
			return (this->_sock);
		}
		const int& getPort() const {
			return (this->_port);
		}
		const std::string& getHostname() const {
			return (this->_hostname);
		}
		sockaddr_in& getSin() {
			return (this->_saddr_in);
		}
		Client& getClient(int i) {
			return (this->_clients[i]);
		}
		std::vector<Client>& getClients() {
			return this->_clients;
		}
		socklen_t& getSinLen() {
			return (this->_saddr_in_len);
		}
		std::map<std::string, Client*>& getClientsM() {
			return this->_clientsM;
		}
		std::map<std::string, Channel>& getChannelsM() {
			return this->_channelsM;
		}
		// Server(int port, std::vector<pollfd>& poll_fds);
		Server(int port);
		Server(int port, int sock, sockaddr_in _saddr_in) : _sock(sock), _port(port), _saddr_in(_saddr_in){};
		~Server();
		void startServer();
		void acceptClient();


		bool handleClientReq(Client &client);

		void handleReqHandshake(Client& client, std::vector<std::string> reqVec);
		void handleReqPing(Client& client, std::vector<std::string> reqVec);
		void handleReqNick(Client& client, std::vector<std::string> reqVec);
		void handleReqUser(Client& client, std::vector<std::string> reqVec);
		void handleReqMode(Client& client, std::vector<std::string> reqVec);
		void handleReqQuit(Client& client, int i);

		void	join(std::vector<std::string> reqVec, Client& client);
		void	part(std::vector<std::string> reqVec, Client& client);
		void	leave(std::vector<std::string> reqVec, Client& client);
		void	nick(std::vector<std::string> reqVec, Client& client);
		void	who(std::vector<std::string> reqVec, Client &client);
		void	quit(std::vector<std::string> reqVec, Client &client);
		void	msg(std::vector<std::string> reqVec, Client& client);
		void	topic(std::vector<std::string> reqVec, Client& client);
		void	mode(std::vector<std::string> reqVec, Client& client);
		void	kick(std::vector<std::string> reqVec, Client& client);
		void	notice(std::vector<std::string> reqVec, Client &client);
		void	invite(std::vector<std::string> reqVec, Client &client);
		void	user(std::vector<std::string> reqVec, Client& client);
		void	ping(std::vector<std::string> reqVec, Client& client);
		void	whois(std::vector<std::string> reqVec, Client &client);
		void	capreq(std::vector<std::string> reqVec, Client &client);
		void	privmsg(std::vector<std::string> reqVec, Client &client);
		void	dcc(std::vector<std::string> reqVeq, Client &client);

		void sendMsgToAllInChannel(Channel &channel, const std::string &response, Client& except);

		void dbgPrint(std::vector<std::string> reqVec, Client &client);

		bool parseReqQueue(Client& client);
		void buildReqQueue(Client &client, const std::string &buffer);

		void disconnectClient(Client& client, int i);
	
		bool parseReq(Client& client, std::string command);

		void broadcastEvent(Client &client, Channel &channel);
		void sendUserList(Client& client, Channel& channel);

		bool isValidClient(std::string name);
		Client& getClientName(std::string name);

		// void sendMsgToAll(std::string message);
		// void sendMsgToAllInChannel(Client &client, std::string message);

	private:
		int	_sock;
		int _port;
		protoent* _proto;
		int _client;
		sockaddr_in _saddr_in;
		socklen_t _saddr_in_len;
		std::string _hostname;
		
		std::vector<Client> _clients;
		std::map<std::string, Client*> _clientsM;

		// std::vector<Channel> _channels;
		std::map<std::string, Channel> _channelsM;

		std::vector<pollfd> _pollFds;
		
		std::map<std::string, void(Server::*)(std::vector<std::string> reqVec, Client& client)> _commands;

		bool checkNick(std::vector<std::string> reqVec, Client& client);
		bool checkUser(std::vector<std::string> reqVec, Client& client);
		bool checkPart(std::vector<std::string> reqVec, Client& client);
		bool checkInvite(std::vector<std::string> reqVec, Client& client);
		bool checkTopic(std::vector<std::string> reqVec, Client& client);
		bool checkJoin(std::vector<std::string> reqVec, Client& client);
};

#endif