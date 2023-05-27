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

std::string msg_3_(std::string host, std::string num, std::string arg1, std::string arg2, std::string arg3)
{
	
	std::string ret_msg = ":" + host + " " + num + " " + arg1 + " " + arg2 + " " + arg3 + "\r\n";
	return ret_msg;
}

bool send_all(int socket, const char *buffer, size_t length)
{
	char *ptr = (char *)buffer;
	while (length > 0)
	{
		int i = send(socket, ptr, length, 0);
		if (i < 1) return false;
		ptr += i;
		length -= i;
	}
	return true;
}

/*std::string	getMsg(int sock, const char *fmt, ...)
{
	va_list	ap;
	char	str[512];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);
	std::string msg = str;
	return msg;
	// std::cout << GRAY << msg << RESET << std::endl;
	// if (send_all(sock, msg.c_str(), msg.size()) == false)
	// 	std::cout << RED << "Error sending message: " << msg << RESET << std::endl;
	// send(sock, msg.c_str(), msg.size(), 0);
}*/