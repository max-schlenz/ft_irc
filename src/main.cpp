#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main()
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1) {
		std::cerr << "cant create socket" << std::endl;
		return (1);
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(5400);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //
	
	return (0);
}