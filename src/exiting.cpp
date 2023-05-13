// #include "irc.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>

void	exiting(int error_code)
{
	if (error_code == 0)
		std::cout << "Invalid Arguments" << std::endl;
	else if (error_code == 1)
		std::cout << "Error establishing connection..." << std::endl;
	else if (error_code == 2)
		std::cout << "Error binding socket..." << std::endl;
	else if (error_code == 3)
		std::cout << "Error on accepting..." << std::endl;
	else if (error_code == 4)
		std::cout << "Error getting proton.." << std::endl;
	else
		std::cout << "ERROR" << std::endl;
	exit(1);
}