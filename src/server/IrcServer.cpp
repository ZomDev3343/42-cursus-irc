# include "../../include/IrcServer.hpp"

IrcServer::IrcServer(int &port, std::string &password)
{
	this->port = port;
	this->password = password;
	this->stopped = false;
}

IrcServer::~IrcServer()
{

}

bool IrcServer::setupServer()
{
	sockaddr_in			sAddr;

	std::cout << "Server is starting..." << std::endl;
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sockfd < 0)
	{
		std::cerr << "Error: Cannot create the socket!" << std::endl;
		return (false);
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = INADDR_ANY;
	sAddr.sin_port = htons(this->port);

	if (bind(this->sockfd, (struct sockaddr*)&sAddr, sizeof(sAddr)) < 0)
	{
		std::cerr << "Error: Cannot bind the socket!" << std::endl;
		return (false);
	}

	listen(this->sockfd, MAX_CLIENTS);

	this->epollfd = epoll_create1(0);
	if (this->epollfd < 0)
	{
		std::cerr << "Error: Cannot create epoll!" << std::endl;
		return (false);
	}

	this->ev.events = EPOLLIN;
	this->ev.data.fd = this->sockfd;

	if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, this->sockfd, &(this->ev)) < 0)
	{
		std::cerr << "Error: Cannot add socket to epoll!" << std::endl;
		return (false);
	}
	std::cout << "Server started!" << std::endl;
	return (true);
}

extern int g_stopSignal;
void IrcServer::serverLoop()
{
	while (!g_stopSignal)
	{

	}
	this->stopServer();
}

void IrcServer::stopServer()
{
	std::cout << "Server is stopping..." << std::endl;
	std::cout << "Server stopped!" << std::endl;
}
