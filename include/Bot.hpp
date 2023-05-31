#ifndef __BOT_HPP
# define __BOT_HPP

# include "Client.hpp"

class Bot: public Client
{
	private:
		int	_specialBot;
	public:
		Bot(int port, std::string ipStr);
		Bot();
};

#endif