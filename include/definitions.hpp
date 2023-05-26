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
// <-------- RESPONSE CODES ---------->

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

// PASS
# define E_NEEDMOREPARAMS(client, str) (std::string(":" SERV_ADDR " 461 ") + client.getNickname()+ " " + str + " " + " :Not enough parameters\r\n")
# define E_ALREADYREGISTERED(client) (std::string(":" SERV_ADDR " 462 ") + client.getNickname()+ " :You may not register\r\n")
# define E_PASSWDMISMATCH(client) (std::string(":" SERV_ADDR " 464 ") + client.getNickname()+ " :Password incorrect\r\n")

// NICK
# define E_NONICKNAMEGIVEN(client) (std::string(":" SERV_ADDR " 431 ") + client.getNickname()+ " :No nickname given\r\n")
# define E_ERRONEUSNICKNAME(client) (std::string(":" SERV_ADDR " 432 ") + client.getNickname()+ " :Erroneus nickname\r\n")
# define E_NICKNAMEINUSE(oldNick, newNick) (std::string(":" SERV_ADDR " 433 ") + oldNick + " " + newNick + " :Nickname is already in use\r\n")

// INVITE
# define R_INVITING(client, nick, channel) (std::string(":" SERV_ADDR " 341 ") + client.getNickname() + " " + nick + " " + channel + "\r\n")
# define R_SENDINV(client, nick, channel) (std::string(":") + client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname() + " INVITE " + nick + " " + channel + "\r\n")

#endif
