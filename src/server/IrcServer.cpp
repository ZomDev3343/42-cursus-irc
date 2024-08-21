# include "../../include/Irc.hpp"
# include "../../include/IrcClient.hpp"
# include "../../include/ACommand.hpp"
# include "../../include/Commands.hpp"
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

	int sockopt = 1;
	setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
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
				this->clients[newsockfd] = new IrcClient(newsockfd);
			}
			else
			{
				char buffer[256] = {0};
				int	user_fd = this->events[i].data.fd;
				int bytes_received = recv(user_fd, buffer, sizeof(buffer), 0);

				if (bytes_received > 0)
				{
					std::cout << "Message recu de [" << user_fd << "]: " << buffer << std::endl;
					this->interpret_message(user_fd, buffer, bytes_received);
				}
				else
				{
					std::cout << "Bye bye mon boug " << this->clients[user_fd]->getId() << std::endl;
					delete this->clients[user_fd];
					this->clients.erase(user_fd);
					close(user_fd);
				}
			}
		}
	}
	this->stopServer();
}

void IrcServer::stopServer()
{
	std::cout << "Server is stopping..." << std::endl;
	for (std::map<int, IrcClient*>::iterator iterator = this->clients.begin(); iterator != this->clients.end(); iterator++)
	{
		close(iterator->first);
		delete iterator->second;
	}
	close(this->sockfd);
	close(this->epollfd);
	std::cout << "Server stopped!" << std::endl;
}

void IrcServer::interpret_message(int user_id, char buffer[256], int const& msglen)
{
	std::string msg_part(buffer, msglen);
	IrcClient* user = this->clients[user_id];
	std::string cmdname;

	// Should never happen
	if (!user){
		std::cerr << "Error: Interpret Message function can't get the client with id [" << user_id << "]" << std::endl; return ;}
	
	if (user->appendMessagePart(msg_part))
	{
		std::string lastmsg = user->getLastMessage();
		cmdname = lastmsg.substr(0, lastmsg.find(' '));
		// TODO -> Execute the function corresponding to 'cmdname'
		user->clearLastMessage();
	}
	else
		std::cout << "INTERPRET MESSAGE: Got just a part of the command !" << std::endl;
}
