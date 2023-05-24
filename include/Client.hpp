
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "common.hpp"
# include "irc.hpp"

typedef struct sockaddr_in sockaddr_in;

class Channel;

class Client
{
	public:
		void setUsername(std::string username){
			this->_userName = username;
		}
		void setNickname(std::string nickname){
			this->_nickName = nickname;
		}
		void setRealname(std::string realname){
			this->_realName = realname;
		}
		void setRegistered(bool registered){
			this->_registered = registered;
		}
		void setModeO(bool mode_o){
			this->_mode_o = mode_o;
		}
		void setModeI(bool mode_i){
			this->_mode_i = mode_i;
		}
		void setIpstr(){
			this->_hostname = inet_ntoa(this->_saddr_in.sin_addr);
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
		const std::string& getUsername() const {
			return (this->_userName);
		}
		const std::string& getNickname() const {
			return (this->_nickName);
		}
		const bool& getModeO() const {
			return (this->_mode_o);
		}
		const bool& getModeI() const {
			return (this->_mode_i);
		}
		const std::string& getHostname() const {
			return (this->_hostname);
		}
		const int& getSock() const {
			return (this->_sock);
		}
		sockaddr_in& getSin() {
			return (this->_saddr_in);
		}
		socklen_t& getSinLen() {
			return (this->_saddr_in_len);
		}
		std::vector<std::string>& getReqQueue() {
			return this->_reqQueue;
		}
		void setReqQueue(std::vector<std::string> reqQueue) {
			this->_reqQueue = reqQueue;
		}
		std::string getUsername() {
			return this->_userName;
		}
		std::string getNickname() {
			return this->_nickName;
		}
		bool getRegistered(){
			return this->_registered;
		}
		std::string getRealName();
		pollfd& getPollFd() {
			return this->_pollFd;
		}

		// std::vector<std::string>& getChannelNames() {
		// 	return this->_channelNames;
		// }
		
		std::map<std::string, Channel*>& getJoinedChannels() {
			return this->_joinedChannels;
		}


		// Client(sockaddr_in sin, socklen_t sinLen, int id, char* ipStr, pollfd& pollFd);
		Client(sockaddr_in sin, int sock, std::string ipStr, pollfd pollFd);
		Client();

	protected:
		std::string _nickName;
		std::string _userName;
		std::string _realName;
		bool		_mode_o;
		bool		_mode_i;
		bool		_bot;
		std::string _hostname;
		int 		_sock;
		sockaddr_in _saddr_in;
		socklen_t _saddr_in_len;
		
		pollfd _pollFd;

		bool _registered;

		std::vector<std::string> _reqQueue;
		// std::vector<Channel*> _joinedChannels;
		// std::vector<std::string> _channelNames;
		std::map<std::string, Channel*> _joinedChannels;
};

#endif