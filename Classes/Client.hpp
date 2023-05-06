#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>

typedef struct sockaddr_in sockaddr_in;

class Client
{
	public:
		const std::string& username() const {
			return (this->_username);
		}
		const std::string& nickname() const {
			return (this->_nickname);
		}
		const bool& is_operator() const {
			return (this->_op);
		}
		const std::string& ipStr() const {
			return (this->_ipStr);
		}
		const int& id() const {
			return (this->_id);
		}
		sockaddr_in& sin() {
			return (this->_sin);
		}
		socklen_t& sinLen() {
			return (this->_sinLen);
		}
		void set_username(std::string username){
			this->_username = username;
		}
		void set_nickname(std::string nickname){
			this->_nickname = nickname;
		}
		void set_op(bool op){
			this->_op = op;
		}
		void set_ipstr(char *ipStr){
			this->_ipStr = ipStr;
		}
		void set_id(int id){
			this->_id = id;
		}
		void set_sin(sockaddr_in& sin){
			this->_sinLen = sizeof(sin);
			this->_sin = sin;
		}
		Client(std::string nickname, std::string username, bool op) : _nickname(nickname), _username(username), _op(op) {}
		Client(){}
		~Client(){}
	private:
		std::string _nickname;
		std::string _username;
		bool		_op;
		std::string _ipStr;
		int 		_id;
		sockaddr_in _sin;
		socklen_t _sinLen;
};

#endif