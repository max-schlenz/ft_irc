
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
bool	checkNick(std::vector<std::string> reqVec, Client& client);
bool	checkUser(std::vector<std::string> reqVec, Client& client);
bool	checkJoin(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels);
bool	channelExists(std::string channelName, std::vector<Channel> channels);
bool	checkPart(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels);
bool	checkInvite(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels, Server& server);

#endif