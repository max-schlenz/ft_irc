# include "irc.hpp"
# include "definitions.hpp"

std::vector<std::string>& getLst(std::string req)
{
	std::string buffer;
	std::vector<std::string> lst;
	std::istringstream iss(req);

	while (getline(iss, buffer, ',')){
		lst.push_back(buffer);
		buffer.clear();
	}
	return lst;
}

bool checkJoin(std::vector<std::string> reqVec, Client& client)
{
	std::string clientIp = client.getIpStr() + " ";
	std::string err_msg;
	std::vector<std::string> channels;
	std::vector<std::string> passwords;

	if (reqVec.size() < 1) {
		err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters";
		send(client.getSock(), err_msg.c_str(), 3, 0);
		return false;
	} else if (reqVec.size() == 1) {
		channels = getLst(reqVec[0]);
	} else if (reqVec.size() == 1) {
		channels = getLst(reqVec[0]);
		passwords = getLst(reqVec[1]);
	} else {
		std::cout << "Join too many arguments" << std::endl;
	}
	return true;
}