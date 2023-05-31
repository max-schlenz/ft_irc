
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

Channel::Channel(std::string topic, std::string password) {
	this->_topic = topic;
	this->_password = password;
}

const std::string& Channel::getTopic() const {
	return (this->_topic);
}

const std::string& Channel::getPassword() const {
	return (this->_password);
}

std::map<std::string, Client*>& Channel::getClientsM() {
	return (this->_clientsM);
}

void Channel::setTopic(std::string topic){
	this->_topic = topic;
}

void Channel::setPassword(std::string password){
	this->_password = password;
}

std::string Channel::getName() {
	return this->_name;
}

void Channel::setLimit(unsigned int limit) {
	this->_limit = limit;
}

void Channel::setModeTrue(char mode) {
	this->_modes[mode] = true;
}

void Channel::setModeFalse(char mode) {
	this->_modes[mode] = false;
}

void Channel::addOperator(std::string name, Client* client) {
	this->_operators[name] = client;
}

void Channel::removeOperator(std::string name) {
	this->_operators.erase(name);
}

unsigned int Channel::getLimit() {
	return this->_limit;
}

std::map<char, bool> Channel::getModes() {
	return this->_modes;
}

std::map<std::string, Client*>& Channel::getOperators() {
	return this->_operators;
}
