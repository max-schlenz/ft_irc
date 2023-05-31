
#include "Channel.hpp"

Channel::Channel(std::string name): _name(name), _topic(""), _password(""), _limit(42)
{
	this->_modes['i'] = false;
	this->_modes['k'] = false;
	this->_modes['t'] = false;
	this->_modes['l'] = false;
	std::cout << "channel: " << name << " created" << std::endl;
}

Channel::Channel(): _name(""), _topic(""), _password(""), _limit(42)
{
	this->_modes['i'] = false;
	this->_modes['k'] = false;
	this->_modes['t'] = false;
	this->_modes['l'] = false;
}
