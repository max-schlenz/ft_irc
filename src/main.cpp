#include "common.hpp"
#include "irc.hpp"

bool g_run = true;

void	error_handling(const char *fmt, ...)
{
	int	errnum;
	va_list	ap;

	errnum = errno;
	va_start(ap, fmt);
	vfprintf(stdout,fmt,ap);
	fprintf(stdout,"\n");
	fflush(stdout);
	if (errnum != 0)
		std::cerr << "errno = (" << errno << ") : " << strerror(errno) << std::endl;
	va_end(ap);
	g_run = false;
}

void sigint_handler(int sig) 
{
	error_handling("Exiting program...");
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
		error_handling("null or empty string argument");
	bool negate = (s[0] == '-');
	if ( *s == '+' || *s == '-' ) 
		++s;
	if ( *s == '\0')
		error_handling("sign character only.");
	while(*s)
	{
		if ( *s < '0' || *s > '9' )
			error_handling("invalid input string.");
		result = result * 10  - (*s - '0');
		++s;
	}
	if ( result > INT_MAX || result < INT_MIN )
		error_handling("invalid number.");
	return negate ? result : -result;
}

int main(int argc, char **argv)
{
	int port = 6667;

	if (argc == 2)
		port = to_int(argv[1]);

	sig();

	Server server(port);
	server.startServer();

	return (0);
}

