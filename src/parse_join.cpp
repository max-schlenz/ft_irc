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

bool channelExists(std::string channelName, std::vector<Channel> channels) {
	for (int i = 0; i < channels.size(); ++i){
		if (channels[i].getName() == channelName)
			return true;
	}
	return false;
}

bool checkJoin(std::vector<std::string> reqVec, Client& client, std::vector<Channel> channels)
{
	std::string clientIp = client.getIpStr() + " ";
	std::string err_msg;
	std::vector<std::string> channels;
	std::vector<std::string> passwords;

	if (reqVec.size() < 1) {
		err_msg = ERR_NEEDMOREPARAMS + clientIp + reqVec[0] + " :Not enough parameters";
		send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
		return false;
	} else if (!channelExists(reqVec[0], channels)) {
		std::cout << "Channel doesnt exist yet, creaate operaator" << std::endl; // logic ist still missing here
	} else if (reqVec.size() == 1) {
		channels = getLst(reqVec[0]);
		for (int i = 0; i < channels.size(); ++i) {
			if (!channelExists(channels[i].getName(), channels))
					//handle what should happen in channel doesnt exist?
					return false
		}
	} else if (reqVec.size() == 2) {
		channels = getLst(reqVec[0]);
		passwords = getLst(reqVec[1]);
		for (int i = 0; i <= channels.size(); ++i) {
			if (!channelExists(channels[i].getName(), channels)
				//handle channel not yet existing
				return false
			if (channels[i].get_password() != passwords[i]) {
				err_msg = ERR_BADCHANNELKEY + clientIp + ":Cannot join channel (+k)";
				send(client.getSock(), err_msg.c_str(), err_msg.size(), 0);
				return false;
			}
			///otherwise join channel? or coomplete knockoff
		}
	} else {
		std::cout << "Join too many arguments" << std::endl;
	}
	return true;
}