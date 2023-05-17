# include "Server.hpp"

void Server::handleReqHandshake(Client& client, std::vector<std::string> reqVec)
{
	// std::cout << "HANDSHAKE" << std::endl;
	// if (client.getNickname() != "default")
	// {
		std::string response = ":127.0.0.1 001 " + client.getNickname() + " :\002\037\0038Welcome to our dmt.irc server!\002\037\0038\r\n";
		send(client.getSock(), response.c_str(), response.size(), 0);
	// }
}

//MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40)
//MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A)
//MUST NOT start with a character listed as a channel type prefix
//SHOULD NOT contain any dot character ('.', 0x2E)