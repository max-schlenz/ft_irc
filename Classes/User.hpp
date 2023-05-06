#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>

class Client
{
	public:
		const std::string& get_username() const {
			return (this->username);
		}
		const std::string& get_nickname() const {
			return (this->nickname);
		}
		const bool& is_operator() const {
			return (this->op);
		}
		void set_username(std::string username){
			this->username = username;
		}
		void set_nickname(std::string nickname){
			this->nickname = nickname;
		}
		void set_op(bool op){
			this->op = op;
		}
		Client(std::string nickname, std::string username, bool op) : nickname(nickname), username(username), op(op) {}
		Client(){}
		~Client(){}
	private:
		std::string nickname;
		std::string username;
		bool		op;
};

#endif