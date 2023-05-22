#include "irc.hpp"

std::string msg_0 (std::string host, std::string num, std::string msg)
{
	
	std::string ret_msg = ":" + host + " " + num + " " + ":" + msg + "\r\n";
	return ret_msg;
}

std::string msg_1 (std::string host, std::string num, std::string arg1, std::string msg)
{
	
	std::string ret_msg = ":" + host + " " + num + " " + arg1 + " " + ":" + msg + "\r\n";
	return ret_msg;
}

std::string msg_2 (std::string host, std::string num, std::string arg1, std::string arg2, std::string msg)
{
	
	std::string ret_msg = ":" + host + " " + num + " " + arg1 + " " + arg2 + " " + ":" + msg + "\r\n";
	return ret_msg;
}

std::string msg_3 (std::string host, std::string num, std::string arg1, std::string arg2, std::string arg3, std::string msg)
{
	
	std::string ret_msg = ":" + host + " " + num + " " + arg1 + " " + arg2 + " " + arg3 + " " + ":" + msg + "\r\n";
	return ret_msg;
}

std::string msg_4 (std::string host, std::string num, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string msg)
{
	
	std::string ret_msg = ":" + host + " " + num + " " + arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " " + ":" + msg + "\r\n";
	return ret_msg;
}