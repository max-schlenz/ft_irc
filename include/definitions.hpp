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


# define RPL_UMODEIS std::string("221")
# define RPL_CHANNELMODEIS std::string("324")
# define RPL_YOUREOPER std::string("381")

# define ERR_NOSUCHNICK std::string("401")
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
# define ERR_UMODEUNKNOWNFLAG std::string("501")
# define ERR_USERSDONTMATCH std::string("502")

# define SERV_ADDR "127.0.0.1"
# define ERR_INVALIDMODEPARAM std::string("696")
// <-------- RESPONSE CODES ---------->

// WHO
# define SHORT_NAME (std::string(":" SERV_ADDR))
# define LONG_NAME(client) (std::string(":") + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname())

// WELCOME
# define NOTICE(...) (std::string(":" SERV_ADDR " NOTICE * :") + __VA_ARGS__)
# define WELCOME_001(client) (std::string(":" SERV_ADDR " 001 ") + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + "\r\n")
# define WELCOME_002(client) (std::string(":" SERV_ADDR " 002 ") + client.getNickname() + " :Your host is ft.irc.de, running version 1.0\r\n")
# define WELCOME_003(client) (std::string(":" SERV_ADDR " 003 ") + client.getNickname() + " :This server was created " + this->_dateString + "\r\n")

# define WELCOME_251(client) (std::string(":" SERV_ADDR " 251 ") + client.getNickname() + " :There are " + itos(this->_clientsM.size()) + " users and 0 invisible on 1 server(s)\r\n")
# define WELCOME_252(client) (std::string(":" SERV_ADDR " 252 ") + client.getNickname() + " " + itos(this->_num_ops) + " :IRC Operators online\r\n")
# define WELCOME_253(client) (std::string(":" SERV_ADDR " 253 ") + client.getNickname() + " 0 :Unknown connection(s)\r\n")
# define WELCOME_254(client) (std::string(":" SERV_ADDR " 254 ") + client.getNickname() + " " + itos(this->_num_channels) + " :Channels formed\r\n")
# define WELCOME_265(client) (std::string(":" SERV_ADDR " 265 ") + client.getNickname() + " " + itos(this->_clientsM.size()) + " " + itos(USR_LIMIT) + " :Current local users " + itos(this->_clientsM.size()) + ", max " + itos(USR_LIMIT) + "\r\n")

# define WELCOME_375(client) (std::string(":" SERV_ADDR " 375 ") + client.getNickname() + " :- Message of the day\r\n")
# define WELCOME_372_1(client) (std::string(":" SERV_ADDR " 372 ") + client.getNickname() + " :- Welcome to our server!\r\n")
# define WELCOME_372_2(client) (std::string(":" SERV_ADDR " 372 ") + client.getNickname() + " :- \r\n")
# define WELCOME_372_3(client) (std::string(":" SERV_ADDR " 372 ") + client.getNickname() + " :- Have fun :)\r\n")
# define WELCOME_376(client) (std::string(":" SERV_ADDR " 376 ") + client.getNickname() + " :End of /MOTD command.\r\n")

# define WELCOME_MODE(client) (std::string(":") + client.getNickname() + " MODE " + client.getNickname() + " :+iw\r\n")

# define WELCOME(client) WELCOME_001(client) + WELCOME_002(client) + WELCOME_003(client) + WELCOME_251(client) + WELCOME_252(client) \
+ WELCOME_253(client) + WELCOME_254(client) + WELCOME_265(client) + WELCOME_375(client) + WELCOME_372_1(client) + WELCOME_372_1(client) \
+ WELCOME_372_2(client) + WELCOME_372_3(client) + WELCOME_376(client) + WELCOME_MODE(client)

// join
# define E_INVITEONLYCHAN(client, channelName) (SHORT_NAME + " 473 " + client.getHostname() + " " + channelName + " :Cannot join channel (+i)\r\n")
//MODES
# define R_CHANNELMODEIS(nickname, channel, modes) (SHORT_NAME + " 324 " + nickname + " " + channel + " " + modes + "\r\n")
# define R_UMODEIS(client, modes) (SHORT_NAME + " 221 " + client.getHostname() + " " + modes + "\r\n")  //client IP needed
# define E_CHANOPRIVSNEEDED(client, channelName) (SHORT_NAME + " 482 " + client.getHostname() + " " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n")
# define USERMODE(client, modeStr) (SHORT_NAME + " MODE " + client.getNickname() + " " + modeStr + ":changing mode\r\n")
# define CHANNELMODE(client, channelName, modeStr) (SHORT_NAME + " MODE " + channelName + " " + modeStr + "\r\n")
# define CHANNELMODEARGS(client, channelName, modeStr, args) (SHORT_NAME + " MODE " + channelName + " " + modeStr + " " + args + "\r\n")
# define E_INVALIDKEY(client, channelName) (SHORT_NAME + " 525 " + channelName + " :Key is not well-formed\r\n")
# define ERR_KEYSET(client, channelName) (SHORT_NAME + " 467 " + channelName + " :Channel key aready set\r\n")
// "<client> <target chan> :Key is not well-formed"

# define E_INVALIDMODEPARAM(client, channelName, mode, str) (SHORT_NAME + " 696 " + client.getHostname() + " " + channelName + " " + mode + " * :" + str + "\r\n")

//topic
# define R_TOPIC(client, channelName, top) (SHORT_NAME + " 332 " + client.getHostname() + " " + channelName + " :" + top + "\r\n") //client IP needed
# define R_NOTOPIC(client, channelName) (SHORT_NAME + " 331 " + client.getHostname() + " " + channelName + " :No topic is set\r\n") //client IP needed

// PASS
# define E_NEEDMOREPARAMS(client, str) (SHORT_NAME + " 461 " + client.getNickname() + " " + str + " " + " :Not enough parameters\r\n")
# define E_ALREADYREGISTERED(client) (SHORT_NAME + " 462 " + client.getNickname() + " :You may not reregister\r\n")
# define E_PASSWDMISMATCH(client) (SHORT_NAME + " 464 " + client.getNickname() + " :Password incorrect\r\n")

// NICK
# define E_NOSUCHNICK(client, nick) (SHORT_NAME + " 401 " + client.getHostname() + " " + nick + " :No such nick\r\n")
# define E_NONICKNAMEGIVEN(client) (SHORT_NAME + " 431 " + client.getNickname() + " :No nickname given\r\n")
# define E_ERRONEUSNICKNAME(client, nick) (SHORT_NAME + " 432 " + client.getHostname() + " " + nick + " :Erroneus nickname\r\n")
# define E_NICKNAMEINUSE(oldNick, newNick) (SHORT_NAME + " 433 " + oldNick + " " + newNick + " :Nickname is already in use\r\n")

// USER
# define E_UMODEUNKNOWNFLAG(client, mode) (SHORT_NAME + " 501 " + client.getNickname() +  " " + mode + " :Unknown MODE flag\r\n") // clientip NOT needed
# define E_USERSDONTMATCH(client) (SHORT_NAME + " 502 " + client.getNickname() + " :Cant change mode for other users\r\n") // clientip NOT needed

// INVITE
# define R_INVITING(client, nick, channel) (SHORT_NAME + " 341 " + client.getNickname() + " " + nick + " " + channel + "\r\n")
# define R_SENDINV(client, nick, channel) (std::string(":") + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname() + " INVITE " + nick + " " + channel + "\r\n")
# define E_USERONCHANNEL(client, nick, channel) (SHORT_NAME + " 443 " + client.getNickname() + " " + nick + " " + channel + " :is already on channel\r\n")

// # define R_ENDOFWHO(client, channel) (std::string(":") + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname() + " 315 " + client.getNickname() + " " + channel + " :End of /WHO list.\r\n")
# define R_ENDOFWHO(client, channel) (SHORT_NAME + " 315 " + " " + channel + " :End of /WHO list.\r\n")

// JOIN
// # define JOIN(client, channel) (std::string(":") + client.getNickname() + " JOIN " + channel + "\r\n")
// # define JOIN(client, channel) (SHORT_NAME + " JOIN " + channel + "\r\n")
# define JOIN(client, channel) (LONG_NAME(client) + " JOIN " + channel + "\r\n") //LONG NAME needed

// PART
# define PART(client, channel) (LONG_NAME(client) + " PART " + channel + "\r\n") //LONG NAME needed
# define E_NOSUCHCHANNEL(client, channel) (SHORT_NAME + " 403 " + client.getNickname() + " " + channel + " :No such channel\r\n")
# define E_NOTONCHANNEL(client, channel) (SHORT_NAME + " 442 " + client.getNickname() + " " + channel + " :You're not on that channel\r\n")

// CHANNEL
# define E_BADCHANNELKEY(client, channel) (SHORT_NAME + " 475 " + client.getNickname() + " " + channel + " :Cannot join channel (+k)\r\n")
# define E_ENDOFNAMES(client, channel) (SHORT_NAME + " 366 " + client.getNickname() + " " + channel + " :End of /NAMES list\r\n")

#endif
