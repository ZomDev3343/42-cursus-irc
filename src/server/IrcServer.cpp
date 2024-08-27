#include "../../include/Irc.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include "../../include/IrcClient.hpp"
#include "../../include/Commands.hpp"
#include "../../include/IrcServer.hpp"

IrcServer::IrcServer(int &port, std::string &password)
{
	this->port = port;
	this->password = password;
	this->stopped = false;
	this->commands["PASS"] = Commands::pass_command;
	this->commands["JOIN"] = Commands::join_command;
	this->commands["NICK"] = Commands::nick_command;
	this->commands["USER"] = Commands::user_command;
	this->commands["PART"] = Commands::part_command;
	this->commands["PRIVMSG"] = Commands::privmsg_command;
}

IrcServer::~IrcServer()
{
}

bool IrcServer::setupServer()
{
	sockaddr_in sAddr;

	std::cout << "Server is starting..." << std::endl;
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sockfd < 0)
	{
		std::cerr << "Error: Cannot create the socket!" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		return (false);
	}

	// Set the socket to non-blocking mode using fcntl
	int flags = fcntl(this->sockfd, F_GETFL, 0);
	if (flags == -1)
	{
		std::cerr << "Error: Cannot get socket flags!" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		return (false);
	}
	if (fcntl(this->sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		std::cerr << "Error: Cannot set socket to non-blocking mode!" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		return (false);
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = INADDR_ANY;
	sAddr.sin_port = htons(this->port);

	if (bind(this->sockfd, (struct sockaddr *)&sAddr, sizeof(sAddr)) < 0)
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
				sockaddr_in client;
				socklen_t cLen = sizeof(client);
				int newsockfd = accept(this->sockfd, (struct sockaddr *)&client, &cLen);
				if (newsockfd < 0)
				{
					std::cerr << "Error: Can't accept client connection!" << std::endl;
					std::cerr << strerror(errno) << std::endl;
					continue;
				}

				char host[NI_MAXHOST];
				if (getnameinfo((struct sockaddr *)&client, cLen, host, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
				{
					close(newsockfd);
					continue;
				}

				this->ev.events = EPOLLIN;
				this->ev.data.fd = newsockfd;
				if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, newsockfd, &ev) < 0)
				{
					std::cerr << "Error: Can't add client to epoll!" << std::endl;
					std::cerr << strerror(errno) << std::endl;
					close(newsockfd);
				}

				std::cout << "Nouveau client !" << std::endl;
				this->clients[newsockfd] = new IrcClient(newsockfd, std::string(host));
			}
			else
			{
				char buffer[256] = {0};
				int user_fd = this->events[i].data.fd;
				int bytes_received = recv(user_fd, buffer, sizeof(buffer) - 1, 0);

				if (bytes_received > 0)
				{
					std::cout << buffer << std::endl;
					std::cout << "Message reçu de [" << user_fd << "]: " << buffer << std::endl;
					this->processMessage(user_fd, buffer);
				}
				else
				{
					std::cout << "Bye bye mon boug " << this->clients[user_fd]->getId() << std::endl;
					this->close_client_connection(user_fd);
				}
			}
		}
	}
	this->stopServer();
}

void IrcServer::processMessage(int user_fd, const char *message)
{
	std::string msg(message);
	std::string buffer;

	// TODO a test
	this->clients[user_fd]->appendToBuffer(msg);
	buffer = this->clients[user_fd]->getBuffer();
	if (buffer.find_first_of("\r\n") == std::string::npos)
		return;

	std::cout << "Pong" << std::endl;

	std::vector<std::string> commands = splitCommands(buffer);
	for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		this->interpret_message(user_fd, *it);
	}
	if (this->clients[user_fd])
		this->clients[user_fd]->clearBuffer();
}

std::vector<std::string> IrcServer::splitCommands(const std::string &msg)
{
	std::vector<std::string> commands;
	std::istringstream stream(msg);
	std::string command;

	while (std::getline(stream, command))
	{
		if (command.find("\r\n") == std::string::npos)
			command += "\r\n";
		commands.push_back(command);
	}

	return commands;
}

void IrcServer::stopServer()
{
	std::cout << "Server is stopping..." << std::endl;
	for (std::map<int, IrcClient *>::iterator iterator = this->clients.begin(); iterator != this->clients.end(); iterator++)
	{
		close(iterator->first);
		delete iterator->second;
	}
	close(this->sockfd);
	close(this->epollfd);
	std::cout << "Server stopped!" << std::endl;
}

void IrcServer::interpret_message(int user_id, std::string const& command)
{
	IrcClient *user = this->clients[user_id];
	std::string cmdname;

	if (!user)
	{
		std::cerr << "Error: Interpret Message function can't get the client with id [" << user_id << "]" << std::endl;
		return;
	}

	CommandFunction cmdf = NULL;
	cmdname = command.substr(0, command.find_first_of(" \r\n"));

	if (cmdname != "CAP" && cmdname != "PASS" && !user->isLogged())
		this->close_client_connection(user_id, "Unauthorized connection, need password!");
	else
	{
		std::cout << "Command Name : " << cmdname << std::endl;
		cmdf = this->commands[cmdname];
		if (cmdf != NULL)
		{
			std::cout << cmdname << " command started..." << std::endl;
			cmdf(*this, *user, command);
			std::cout << cmdname << " command finished..." << std::endl;
		}
	}
}

std::vector<Channel *> IrcServer::getChannels()
{
	return (this->_channels);
}

void	IrcServer::addChannel(Channel* channel)
{
	this->_channels.push_back(channel);
}

Channel *IrcServer::getChannel(std::string name)
{
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if ((*it)->getName() == name)
			return (*it);
	}
	return (NULL);
}

IrcClient *IrcServer::getClient(std::string nickname)
{
	for (std::map<int, IrcClient *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return (NULL);
}

std::string const& IrcServer::getPassword() const
{
	return this->password;
}

void IrcServer::close_client_connection(int user_id, std::string reason)
{
	if (this->clients[user_id] != NULL)
	{
		delete this->clients[user_id];
		this->clients.erase(user_id);
		close(user_id);
		if (!reason.empty())
			std::cout << "Kicked User " << user_id << " because " << reason << std::endl;
	}
}
