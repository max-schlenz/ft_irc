#pragma once

#ifndef __DICEBOT__
# define __DICEBOT__

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <poll.h>
# include <iostream>
# include <algorithm>
# include <vector>
# include <cstdlib>
# include <ctime>
# include <iomanip>
# include <iostream>
# include <csignal>

# define RECV_BUF 1024
# define BOLD "\033[1m"
# define RED "\033[0;31m"
# define BRED "\033[1;31m"
# define GREEN "\033[0;32m"
# define BGREEN "\033[1;32m"
# define BLUE "\033[0;34m"
# define BBLUE "\033[1;34m"
# define GRAY "\033[0m\033[38;5;239m"
# define BWHITE "\033[1;37m"
# define YELLOW "\033[0;33m"
# define PINK "\033[38;5;206m"
# define RESET "\033[0m"

class DiceBot
{
	private:
					int			_sock;
					sockaddr_in _hostname;
					std::string	_channel;
					pollfd		_pollFds[1];

	public:
					DiceBot(std::string ip, int port, std::string channel);
					~DiceBot();

					int getSock();
					void registerWithServer();
					bool listenToServer();
					int roll(int n);
					int recievedRoll(std::string message);
};

#endif