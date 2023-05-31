
#include "DiceBot.hpp"

bool g_run = true;

void sigint_handler(int sig) 
{
	g_run = false;
}

static bool parseChannel(std::string channelName)
{
	if (channelName.size() > 1 && channelName[0] == '#')
		return true;
	return false;
}

int main(int argc, char** argv)
{
	if (argc != 4)
		return 1;
	
	signal(SIGINT, sigint_handler);

	if (parseChannel(argv[3]))
	{
		DiceBot dicebot("127.0.0.1", 6667, argv[3]);

		dicebot.registerWithServer();
		dicebot.listenToServer();
	}
    return 0;
}
