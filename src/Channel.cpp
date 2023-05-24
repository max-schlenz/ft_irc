
#include "Channel.hpp"

Channel::Channel(std::string name):
	_name(name), _topic(""), _password("1234"), _modes(""), mode_i(false), mode_t(false), mode_k(true), mode_o(false)
{
	std::cout << "channel: " << name << " created" << std::endl;
}

Channel::~Channel()
{
	;
}