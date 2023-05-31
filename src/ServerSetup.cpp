# include "Server.hpp"

Server::Server(int port, std::string key)
{
	this->_clients.reserve(1024);
	this->_pollFds.reserve(1024);

	if (key != "")
	{
		this->_key = key;
		this->_key_set = true;
	}
	else
		this->_key_set = false;
	if (((this->_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ))
		error_handling("[-] Error, socket invalid!");
	std::cout << GREEN << "[+] Socket " << this->_sock << " is open through the TCP/IP protocol." << RESET << std::endl;
	this->_port = port;
	this->_saddr_in.sin_family = AF_INET;
	this->_saddr_in.sin_port = htons(this->_port);
	this->_saddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_saddr_in_len = sizeof(this->_saddr_in);
	this->_hostname = inet_ntoa(this->_saddr_in.sin_addr);
	this->_num_channels = 0;
	this->_num_ops = 0;
	this->_num_users = 0;

	int optval = 1;
    setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	fcntl(this->_sock, F_SETFL, O_NONBLOCK);
	while ((bind(this->_sock, (struct sockaddr*)&this->_saddr_in, this->_saddr_in_len)) < 0)
	{
		std::cout << YELLOW << "\terrno = (" << errno << ") : " << strerror(errno) << std::endl;
		this->_saddr_in.sin_port = htons(++this->_port);
	}
	std::cout << GREEN << "[+] Bind successful on port " << this->_port << "!" << RESET << std::endl;
	if ((listen(this->_sock, USR_LIMIT)) < 0)
		error_handling("Error: listen failed!");
	std::cout << GREEN << "[+] Socket is listening.\n" << RESET << std::endl;
	pollfd server_poll_fd;
	server_poll_fd.fd = this->_sock;
	server_poll_fd.events = POLLIN;
	time_t currentTime = time(NULL);
	this->_dateString = ctime(&currentTime);
	this->_pollFds.push_back(server_poll_fd);
	this->setCommands();
	this->_bot_on = false;
}

Server::~Server()
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		close((*it).getSock());
	close(this->_sock);
}

void Server::setCommands()
{
	this->_commands["JOIN"] = &Server::join;
	this->_commands["PART"] = &Server::part;
	this->_commands["LEAVE"] = &Server::part;
	this->_commands["NICK"] = &Server::nick;
	this->_commands["TOPIC"] = &Server::topic;
	this->_commands["MODE"] = &Server::mode;
	this->_commands["KICK"] = &Server::kick;
	this->_commands["INVITE"] = &Server::invite;
	this->_commands["USER"] = &Server::user;
	this->_commands["PING"] = &Server::ping;
	this->_commands["WHOIS"] = &Server::whois;
	this->_commands["CAP"] = &Server::capreq;
	this->_commands["PASS"] = &Server::pass;
	this->_commands["PRIVMSG"] = &Server::privmsg;
	this->_commands["MSG"] = &Server::privmsg;
	this->_commands["WHO"] = &Server::who;
	this->_commands["NOTICE"] = &Server::notice;
	this->_commands["QUIT"] = &Server::quit;
	this->_commands["BOT"] = &Server::bot;
	this->_commands["DCC"] = &Server::dcc;

	this->_commands["DBG"] = &Server::dbgPrint;
}