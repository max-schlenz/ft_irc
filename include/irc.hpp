
#ifndef IRC_H
# define IRC_H

# include "common.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "definitions.hpp"

# define RED "\033[0;31m"
# define BRED "\033[1;31m"
# define GREEN "\033[0;32m"
# define BGREEN "\033[1;32m"
# define BLUE "\033[0;34m"
# define BBLUE "\033[1;34m"
# define GRAY "\033[0m\033[38;5;239m"
# define BWHITE "\033[1;37m"
# define RESET "\033[0m"

# define VERBOSE 0

# define PRNT std::cout << BRED << "HERE" << RESET << std::endl
 
class Server;

void	exiting(int error_code);
bool	channelExists(std::string channelName, std::vector<Channel> channels);
std::string itos(const int i);
void printReqVec(std::vector<std::string> reqVec);

std::string msg_0 (std::string host, std::string num, std::string msg);
std::string msg_1 (std::string host, std::string num, std::string arg1, std::string msg);
std::string msg_2 (std::string host, std::string num, std::string arg1, std::string arg2, std::string msg);
std::string msg_3 (std::string host, std::string num, std::string arg1, std::string arg2, std::string arg3, std::string msg);
std::string msg_4 (std::string host, std::string num, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string msg);

#endif