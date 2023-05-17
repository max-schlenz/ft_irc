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
	if (reqVec.size() > 1) {
		send(client.getSock(), ERR_INVALIDNICK, 3, 0);
		return false;
	}
	else if (reqVec.size() < 1) {
		send(client.getSock(), ERR_NONICK, 3, 0);
		return false;
	}
	else if (!checkChars(name)) {
		send(client.getSock(), ERR_INVALIDNICK, 3, 0);
		return false;
	}
	else if (name.size() > 9)
		std::cout << "SIZE greater 9: what to do" << std::endl;
	return true;
}

bool checkUser(std::vector<std::string> reqVec, Client& client)
{
	if (client.getUsername().size() < 0) {
		send(client.getSock(), ERR_ALREADYREGISTERED, 3, 0);
		return false;
	} else {
		if (reqVec.size() < 1) {
			send(client.getSock(), ERR_NEEDMOREPARAMS, 3, 0);
			return false;
		}
	}
	return true;
}

bool checkJoin(std::vector<std::string> reqVec, Client& client)
{
	if (reqVec.size() < 1) {
		send(client.getSock(), ERR_NEEDMOREPARAMS, 3, 0);
		return false;
	}
	return true;
}

