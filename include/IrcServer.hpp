#pragma once

#define MAX_CLIENTS 10
#include "Channel.hpp"
#include <sys/epoll.h>

class IrcClient;
class IrcServer;
class Channel;
typedef void (*CommandFunction)(IrcServer &server, IrcClient &user, std::string command);
class IrcServer
{
private:
	int port;
	std::string password;
	int sockfd;
	int epollfd;
	struct epoll_event ev;
	struct epoll_event events[MAX_CLIENTS];
	bool stopped;
	std::map<int, IrcClient *> clients;
	std::map<std::string, CommandFunction> commands;
	std::vector<Channel *> _channels;

	IrcServer();

public:
	IrcServer(int &port, std::string &password);
	~IrcServer();

	bool setupServer();
	void serverLoop();
	void stopServer();
	void addChannel(Channel* channel);
	void processMessage(int user_fd, const char *message);
	std::vector<std::string> splitCommands(const std::string &msg);
	std::vector<Channel *> getChannels();
	IrcClient *getClient(std::string nickname);
	Channel *getChannel(std::string name);
	std::string const& getPassword() const;

	void	interpret_message(int user_id, const char buffer[256], const int &msglen);
	void	close_client_connection(int user_id, std::string reason = "");

};
