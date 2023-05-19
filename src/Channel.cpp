
#include "Channel.hpp"

Channel::Channel(std::string name):
	_name(name), _topic(), _password(), _modes()
{
	std::cout << "channel: " << name << " created" << std::endl;
}

Channel::~Channel()
{
	std::cout << GRAY << "   + removed channel: " << RESET << this->_name << std::endl;
}