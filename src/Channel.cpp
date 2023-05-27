
#include "Channel.hpp"

Channel::Channel(std::string name): _name(name), _topic(""), _password("")
{
	this->_modes['i'] = false;
	this->_modes['k'] = false;
	this->_modes['t'] = false;
	std::cout << "channel: " << name << " created" << std::endl;
}

Channel::Channel(): _name(""), _topic(""), _password("")
{
	this->_modes['i'] = false;
	this->_modes['k'] = false;
	this->_modes['t'] = false;
}

Channel::~Channel()
{
	;
}