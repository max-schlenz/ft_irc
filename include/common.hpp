#ifndef COMMON_H
# define COMMON_H

// CPP
# include <algorithm>
# include <cstdlib>
# include <cstring>
# include <iomanip>
# include <iostream>
# include <fstream>
# include <map>
# include <sstream>
# include <stdexcept>
# include <string>
# include <vector>
# include <csignal>
# include <cstdio>

// C
# include <arpa/inet.h>
# include <netinet/in.h>
# include <poll.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <netdb.h>
# include <fcntl.h>

void	error_handling(const char *fmt, ...);

#endif
