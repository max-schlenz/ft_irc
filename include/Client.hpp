
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "common.hpp"
# include "Channel.hpp"

typedef struct sockaddr_in sockaddr_in;

class Channel;

class Client
{
	public:

	Client(sockaddr_in sin, int sock, std::string ipStr, pollfd pollFd);
	Client();

	void 								setUsername(std::string username);	
	void 								setNickname(std::string nickname);
	void 								setRealname(std::string realname);
	void 								setNickRegistered(bool registered);
	void 								setUserRegistered(bool registered);
	void 								setModeI(bool mode_i);
	void 								setIpstr();
	void 								setSock(int sock);
	void 								setKick(bool kick);
	void 								setPass(bool pass);
	void 								setSin(sockaddr_in& sin);
	void 								setSinLen(socklen_t sinLen);
	void								setReqQueue(std::vector<std::string> reqQueue); 
	void								setConnected(bool connected);

	const bool& 						getModeI() const;
	const bool&							getPass() const;
	const bool&							getKick() const;
	const int&							getSock() const;

	sockaddr_in& 						getSin();
	socklen_t&							getSinLen();
	const std::string&					getUsername() const; 
	const std::string&					getNickname() const;
	const std::string&					getHostname() const;
	bool								getNickRegistered();
	bool								getUserRegistered();
	bool								getConnected();
	
	std::string&						getReqQueueBuf();
	std::string&						getRealName();
	pollfd&								getPollFd();

	std::vector<std::string>&			getReqQueue(); 
	std::map<std::string, Channel*>&	getJoinedChannels();

	void								createBot();

	protected:
		std::string 					_nickName;
		std::string 					_userName;
		std::string 					_realName;
		std::string 					_hostname;

		bool							_mode_i;
		bool							_bot;
		bool							_kick;
		bool							_pass;
		bool							_connected;
		bool							_nick_registered;
		bool							_user_registered;
		
		int 							_sock;
		sockaddr_in 					_saddr_in;
		socklen_t						_saddr_in_len;
		pollfd							_pollFd;
		
		std::string						_reqQueueBuf;
		std::vector<std::string>		_reqQueue;
		std::map<std::string, Channel*>	_joinedChannels;
};

#endif