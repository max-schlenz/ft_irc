#include "common.hpp"
# include "Server.hpp"
#include <climits>
#include <cstdarg>

bool g_run = true;
bool g_bot = false;

void	error_handling(const char *fmt, ...)
{
	int					errnum;
	va_list				ap;
	std::ostringstream	oss;

	errnum = errno;
	va_start(ap, fmt);
	while (*fmt != '\0')
	{
		if (*fmt == '%' && *(fmt + 1) == 's')
		{
			const char *arg = va_arg(ap, const char*);
			oss << arg;
			fmt += 2;
		}
		if (*fmt == '%' && *(fmt + 1) == 'i')
		{
			int	arg = va_arg(ap, int);
			oss << arg;
			fmt += 2;
		}
		else
		{
			oss << *fmt;
			fmt++;
		}
	}
	va_end(ap);
	std::cerr << oss.str() << std::endl;
	if (errnum != 0)
		std::cerr << "errno = (" << errno << ") : " << strerror(errno) << std::endl;
	g_run = false;
}

void sigint_handler(int sig) 
{
	std::cout << YELLOW << "\nExiting program..." << RESET << std::endl;
	g_run = false;
}

void sig()
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR) 
		error_handling("Error: Unable to register signal handler!");
}

int to_int(char const *s)
{
	long long result = 0;

	if ( s == NULL || *s == '\0' )
	{
		std::cout << BRED << "null or empty string argument." << std::endl;
		return -1;
	}
	while(*s)
	{
		if ( *s < '0' || *s > '9' )
		{
			std::cout << BRED << "invalid input string." << std::endl;
			return -1;
		}
		result = result * 10  - (*s - '0');
		++s;
	}
	if ( result > INT_MAX || result < INT_MIN )
		{
			std::cout << BRED << "invalid number." << std::endl;
			return -1;
		}
	return -result;
}

int main(int argc, char **argv)
{
	int port = 6667;
	std::string key;

	if (argc > 3)
	{
		std::cout << BRED << "\nError!\n" << RED << "[Usage]:\t./ircserv <port> <password>\n" << RESET << std::endl;
		return (0);
	}
	else if (argc == 1)
	{
		std::cout << BRED << "Error!\n" << RED << "[Usage]: ./ircserv <port> <password>\n" << RESET << std::endl;
		std::cout << YELLOW << "Starting on default port 6667 without a password!" << std::endl;
		std::cout << "Exit here if you don't want this behavior!\n" << RESET << std::endl;
	}
	else
		port = to_int(argv[1]);
	if (port == -1)
		return (0);
	if (argc == 3)
	{
		std::cout << "\nPassword required!\n" << std::endl;
		key = argv[2];
	}
	else
	{
		std::cout << "No password required!\n" << std::endl;
		key = "";
	}
	sig();
	Server server(port, key);
	server.startServer();

	return (0);
}

