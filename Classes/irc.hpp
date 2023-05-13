#ifndef IRC_H
# define IRC_H
# include <string>
# include <iostream>
# include <cstdlib>
# include "Server.hpp"

# define RED "\033[0;31m"
# define BRED "\033[1;31m"
# define GREEN "\033[0;32m"
# define BGREEN "\033[1;32m"
# define BLUE "\033[0;34m"
# define BBLUE "\033[1;34m"
# define GRAY "\033[0m\033[38;5;239m"
# define BWHITE "\033[1;37m"
# define RESET "\033[0m"

void	exiting(int error_code);
void	join(std::string cmd);
void	leave(std::string cmd);
void	nick(std::string cmd);
void	quit(std::string cmd);
void	msg(std::string cmd);
void	topic(std::string cmd);
void	mode(std::string cmd);
void	kick(std::string cmd);
void	invite(std::string cmd);

#endif