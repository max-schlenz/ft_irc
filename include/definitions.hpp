#ifndef DEFINITIONS_HPP
# define DEFINITIONS_HPP

# include "common.hpp"

# define RPL_WELCOME 001
# define RPL_YOURHOST 002
# define RPL_CREATED 003
# define RPL_MYINFO 004
# define RPL_ISUPPORT 005
# define RPL_TOPIC 332
# define RPL_NAMREPLY 353
# define RPL_ENDOFNAMES 366
# define ERR_ERRONEUSNICKNAME 432

#define ERR_NOSUCHNICK std::string("401")
# define ERR_NOSUCHCHANNEL std::string("403")
# define ERR_NONICK std::string("431")
# define ERR_INVALIDNICK std::string("432")
# define ERR_NICKINUSE std::string("433")
# define ERR_USERNOTINCHANNEL std::string("441")
# define ERR_NOTONCHANNEL std::string("442") 
# define ERR_NEEDMOREPARAMS std::string("461")
# define ERR_ALREADYREGISTERED std::string("462")
# define ERR_INVITEONLYCHAN std::string("473")
# define ERR_BADCHANNELKEY std::string("475")
# define ERR_CHANOPRIVSNEEDED std::string("482")
# define ERR_USERONCHANNEL std::string("443") 
# define ERR_CHANOPRIVSNEEDED std::string("482")

#endif
