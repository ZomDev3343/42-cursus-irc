#pragma once

# define MAX_CLIENTS 10
# include "Channel.hpp"
#include <sys/epoll.h>

class IrcClient;
class IrcServer;
class Channel;
typedef void (*CommandFunction)(IrcServer &server, IrcClient &user, std::string command);
class IrcServer
{
	private:
		int										port;
		std::string 							password;
		int										sockfd;
		int										epollfd;
		struct epoll_event						ev;
		struct epoll_event						events[MAX_CLIENTS];
		bool									stopped;
		std::map<int, IrcClient*>				clients;
		std::map<std::string, CommandFunction>	commands;
		std::vector<Channel*>					_channels;

		IrcServer();
	public:
		IrcServer(int &port, std::string &password);
		~IrcServer();

		bool	setupServer();
		void	serverLoop();
		void	stopServer();
		std::vector<Channel*> getChannels();
		Channel *getChannel(std::string name);

		void	interpret_message(int user_id, char buffer[256], int const& msglen);
};
