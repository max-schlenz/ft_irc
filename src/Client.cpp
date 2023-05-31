#include "Client.hpp"
#include "Server.hpp"

Client::Client(sockaddr_in sin, int sock, std::string ipStr, pollfd pollFd): 
	_nickName("default"), _userName("default"), _realName("default"), _hostname(ipStr), _mode_i(true), _bot(false), _kick(false), _pass(false), _connected(true), _nick_registered(false), _user_registered(false), _sock(sock), _saddr_in(sin), _saddr_in_len(sizeof(sin)), _pollFd(pollFd), _reqQueueBuf(), _reqQueue(), _joinedChannels() 
{
	std::cout << GREEN << "Client " << BGREEN << this->_hostname << GREEN << " connected." << RESET << std::endl;
}

Client::Client(){}

void Client::setUsername(std::string username){
	this->_userName = username;
}

void Client::setNickname(std::string nickname){
	this->_nickName = nickname;
}

void Client::setRealname(std::string realname){
	this->_realName = realname;
}

void Client::setNickRegistered(bool registered){
	this->_nick_registered = registered;
}

void Client::setUserRegistered(bool registered){
	this->_user_registered = registered;
}

void Client::setModeI(bool mode_i){
	this->_mode_i = mode_i;
}

void Client::setIpstr(){
	this->_hostname = inet_ntoa(this->_saddr_in.sin_addr);
}

void Client::setSock(int sock){
	this->_sock = sock;
}

void Client::setKick(bool kick){
	this->_kick = kick;
}

void Client::setPass(bool pass){
	this->_pass = pass;
}

void Client::setSin(sockaddr_in& sin){
	this->_saddr_in_len = sizeof(sin);
	this->_saddr_in = sin;
}

void Client::setSinLen(socklen_t sinLen) {
	this->_saddr_in_len = sinLen;
}

const bool& Client::getModeI() const {
	return (this->_mode_i);
}

const bool& Client::getPass() const {
	return (this->_pass);
}

const bool& Client::getKick() const {
	return (this->_kick);
}

const std::string& Client::getHostname() const {
	return (this->_hostname);
}

std::string& Client::getRealName()
{
	return this->_realName;
}

const int& Client::getSock() const {
	return (this->_sock);
}

sockaddr_in& Client::getSin() {
	return (this->_saddr_in);
}

socklen_t& Client::getSinLen() {
	return (this->_saddr_in_len);
}

std::vector<std::string>& Client::getReqQueue() {
	return this->_reqQueue;
}

void Client::setReqQueue(std::vector<std::string> reqQueue) {
	this->_reqQueue = reqQueue;
}

const std::string& Client::getUsername() const {
	return this->_userName;
}

const std::string& Client::getNickname() const {
	return this->_nickName;
}

bool Client::getNickRegistered(){
	return this->_nick_registered;
}

bool Client::getUserRegistered(){
	return this->_user_registered;
}

std::string& Client::getReqQueueBuf(){
	return this->_reqQueueBuf;
}

pollfd& Client::getPollFd() {
	return this->_pollFd;
}

void Client::setConnected(bool connected){
	this->_connected = connected;
}
bool Client::getConnected(){
	return this->_connected;
}

std::map<std::string, Channel*>& Client::getJoinedChannels() {
	return this->_joinedChannels;
}
