#include "AntiSpam.hpp"

AntiSpamBot::AntiSpamBot(): Client()
{
	this->_bot = true;
	this->_op = false;
	this->_nickName = "AntiSpam";
	this->_userName = "AntiSpam";
	this->_realName = "AntiSpam";

	std::cout << GREEN << "AntiSpam-Bot constructed" << RESET << std::endl;
}

