#include "Server.hpp"

void join(std::vector<std::string> reqVec)
{
	std::cout << "JOIN" << std::endl;
}

void leave(std::vector<std::string> reqVec)
{
	std::cout << "leave" << std::endl;
}

void nick(std::vector<std::string> reqVec)
{
	std::cout << "nick" << std::endl;
}

void quit(std::vector<std::string> reqVec)
{
	std::cout << "quit" << std::endl;
}

void msg(std::vector<std::string> reqVec)
{
	std::cout << "msg" << std::endl;
}

void topic(std::vector<std::string> reqVec)
{
	std::cout << "topic" << std::endl;
}

void mode(std::vector<std::string> reqVec)
{
	std::cout << "mode" << std::endl;
}

void kick(std::vector<std::string> reqVec)
{
	std::cout << "kick" << std::endl;
}

void invite(std::vector<std::string> reqVec)
{
	std::cout << "invite" << std::endl;
}

void ping(std::vector<std::string> reqVec)
{
	std::cout << "ping" << std::endl;
}

void user(std::vector<std::string> reqVec)
{
	std::cout << "user" << std::endl;
}