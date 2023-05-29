
#include "DiceBot.hpp"

bool g_run = true;

void sigint_handler(int sig) 
{
	g_run = false;
}

int main(int argc, char** argv)
{
	signal(SIGINT, sigint_handler);

	DiceBot dicebot("127.0.0.1", 6667, "#test");

	dicebot.registerWithServer();
	dicebot.listenToServer();


    return 0;
}
