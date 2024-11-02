#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#define MAX_CLIENTS 10
#include "Channel.hpp"
#include <sys/epoll.h>

class IrcClient;
class Bot;
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
	Bot* bot;

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

	void	interpret_message(int user_id, std::string const& command);
	void	close_client_connection(int user_id, std::string reason = "");

	Bot* getBot();
	void sendMessageFromBot(IrcClient* bot, const std::string& targetName, const std::string& message);

};

#endif
