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

static int to_int(char const *s)
{
	long long result = 0;

	if ( s == NULL || *s == '\0' )
		return(error_handling("null or empty string argument"), -1);
	bool negate = (s[0] == '-');
	if ( *s == '+' || *s == '-' ) 
		++s;
	if ( *s == '\0')
		return(error_handling("sign character only."), -1);
	while(*s)
	{
		if ( *s < '0' || *s > '9' )
			return(error_handling("invalid input string."), -1);
		result = result * 10  - (*s - '0');
		++s;
	}
	if ( result > INT_MAX || result < INT_MIN )
		return(error_handling("invalid number."), -1);
	return negate ? result : -result;
}

int main(int argc, char **argv)
{
	int port = 6667;
	std::string key;

	if (argc == 1)
	{
		std::cout << BRED << "Error!\n" << RED << "[Usage]: ./ircserv <port> <password>\n" << RESET << std::endl;
		std::cout << YELLOW << "Starting on default port 6667 without a password!" << std::endl;
		std::cout << "Exit here if you don't want this behavior!\n" << RESET << std::endl;
	}
	else
		port = to_int(argv[1]);
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

