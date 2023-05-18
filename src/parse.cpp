# include "irc.hpp"
# include "definitions.hpp"

static bool checkChars(std::string name)
{
	std::string notAllowed = ",*?!@.";
	std::string notStart = ":$#&";
	for (int i = 0; i < notAllowed.size(); ++i) {
		if (name.find(notAllowed[i]))
			return false;
	}
	if (notStart.find(name[0]))
		return false;
	return true;
}

bool checkNick(std::vector<std::string> reqVec, Client& client)
{
	std::string name = reqVec[0];
	std::string clientIp = client.getIpStr() + " ";
	std::string currentNick = client.getNickname() + " ";
	std::string err_msg;
	if (reqVec.size() > 1) {
		err_msg =  ERR_INVALIDNICK + clientIp + currentNick + ":Erroneus nickname\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	else if (reqVec.size() < 1) {
		err_msg = ERR_NONICK + clientIp + ":No nickname given\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	else if (!checkChars(name)) {
		err_msg =  ERR_INVALIDNICK + clientIp + currentNick + ":Erroneus nickname\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	}
	else if (name.size() > 9)
		std::cout << "SIZE greater 9: what to do" << std::endl;
	return true;
}

bool checkUser(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getIpStr() + " ";
	std::string err_msg;
	if (client.getUsername().size() < 0) {
		err_msg = ERR_ALREADYREGISTERED + ":You may not reregister\r\n";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	} else {
		if (reqVec.size() < 1) {
			err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters";
			send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
			return false;
		}
	}
	return true;
}



