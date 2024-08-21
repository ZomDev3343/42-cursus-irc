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
		std::cerr << strerror(errno) << std::endl;
		return (false);
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = INADDR_ANY;
	sAddr.sin_port = htons(this->port);

	if (bind(this->sockfd, (struct sockaddr*)&sAddr, sizeof(sAddr)) < 0)
	{
		std::cerr << "Error: Cannot bind the socket!" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		close(this->sockfd);
		return (false);
	}

	listen(this->sockfd, MAX_CLIENTS);

	this->epollfd = epoll_create1(0);
	if (this->epollfd < 0)
	{
		std::cerr << "Error: Cannot create epoll!" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		close(this->sockfd);
		return (false);
	}

	this->ev.events = EPOLLIN;
	this->ev.data.fd = this->sockfd;

	if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, this->sockfd, &(this->ev)) < 0)
	{
		std::cerr << "Error: Cannot add socket to epoll!" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		close(this->sockfd);
		close(this->epollfd);
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
		int nfds = epoll_wait(this->epollfd, this->events, MAX_CLIENTS, -1);
		for (int i = 0; i < nfds; ++i)
		{
			if (this->events[i].data.fd == this->sockfd)
			{
				sockaddr_in	client;
				socklen_t	cLen = sizeof(client);
				int			newsockfd = accept(this->sockfd, (struct sockaddr*)&client, &cLen);
				if (newsockfd < 0)
				{
					std::cerr << "Error: Can't accept client connection!" << std::endl;
					std::cerr << strerror(errno) << std::endl;
					continue ;
				}

				this->ev.events = EPOLLIN;
				this->ev.data.fd = newsockfd;
				if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, newsockfd, &ev) < 0)
				{
					std::cerr << "Error: Can't add client to epoll!" << std::endl;
					std::cerr << strerror(errno) << std::endl;
					close(newsockfd);
				}
				std::cout << "New client connected!" << std::endl;
				this->clients[newsockfd] = IrcClient(newsockfd)
			}
			else
			{
				char buffer[128] = {0};
				int bytes_received = recv(this->events[i].data.fd, buffer, sizeof(buffer), 0);
				if (bytes_received > 0)
					std::cout << "Message recu de " << this->events[i].data.fd << ": " << buffer << std::endl;
				else
				{
					std::cout << "Bye bye mon boug!" << std::endl;
					this->clients.erase(this->events[i].data.fd);
					close(this->events[i].data.fd);
				}
			}
		}
	}
	this->stopServer();
}

void IrcServer::stopServer()
{
	std::cout << "Server is stopping..." << std::endl;
	typedef std::map<int, IrcClient>::iterator it;
	for (it iterator = this->clients.begin(); iterator != this->clients.end(); iterator++)
		close(iterator->first);
	this->clients.clear();
	close(this->sockfd);
	close(this->epollfd);
	std::cout << "Server stopped!" << std::endl;
}
