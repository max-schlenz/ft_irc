#include "Server.hpp"

void join(std::string rest)
{
	std::cout << "JOIN" << std::endl;
}

void leave(std::string cmd)
{
	std::cout << "leave" << std::endl;
}

void nick(std::string cmd)
{
	std::cout << "nick" << std::endl;
}

void quit(std::string cmd)
{
	std::cout << "quit" << std::endl;
}

void msg(std::string cmd)
{
	std::cout << "msg" << std::endl;
}

void topic(std::string cmd)
{
	std::cout << "topic" << std::endl;
}

void mode(std::string cmd)
{
	std::cout << "mode" << std::endl;
}

void kick(std::string cmd)
{
	std::cout << "kick" << std::endl;
}

void invite(std::string cmd)
{
	std::cout << "invite" << std::endl;
}