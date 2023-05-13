#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>
# include <iostream>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>
# include <poll.h>

typedef struct sockaddr_in sockaddr_in;

class Client
{
	public:
		void setUsername(std::string username){
			this->_userName = username;
		}
		void setNickname(std::string nickname){
			this->_nickName = nickname;
		}
		void setOp(bool op){
			this->_op = op;
		}
		void setIpstr(){
			this->_ipStr = inet_ntoa(this->_saddr_in.sin_addr);
		}
		void setSock(int sock){
			this->_sock = sock;
		}
		void setSin(sockaddr_in& sin){
			this->_saddr_in_len = sizeof(sin);
			this->_saddr_in = sin;
		}
		void setSinLen(socklen_t sinLen) {
			this->_saddr_in_len = sinLen;
		}
		const std::string& username() const {
			return (this->_userName);
		}
		const std::string& nickname() const {
			return (this->_nickName);
		}
		const bool& is_operator() const {
			return (this->_op);
		}
		const std::string& ipStr() const {
			return (this->_ipStr);
		}
		const int& sock() const {
			return (this->_sock);
		}
		sockaddr_in& sin() {
			return (this->_saddr_in);
		}
		socklen_t& sinLen() {
			return (this->_saddr_in_len);
		}
		std::vector<std::string>& getCmdQueue() {
			return this->_cmdQueue;
		}
		void setCmdQueue(std::vector<std::string> cmdQueue) {
			this->_cmdQueue = cmdQueue;
		}
		std::string& getUsername() {
			return this->_userName;
		}
		std::string& getNickname() {
			return this->_nickName;
		}
		// void setPollFd(pollfd& pollFd) {
		// 	this->_pollFd = pollFd;
		// }
		pollfd getPollFd() {
			return this->_pollFd;
		}
		// Client(std::string nickname, std::string username, bool op, pollfd& pollFd) : _nickName(nickname), _userName(username), _op(op) {}
		Client(sockaddr_in sin, socklen_t sinLen, int id, char* ipStr, pollfd& pollFd);
		// Client& operator=(const Client& other);
		// Client(){}
		// ~Client(){}
	private:
		std::string _nickName;
		std::string _userName;
		std::string _realName;
		bool		_op;
		std::string _ipStr;
		int 		_sock;
		sockaddr_in _saddr_in;
		socklen_t _saddr_in_len;
		std::vector<std::string> _cmdQueue;

		pollfd _pollFd;
		
};

#endif