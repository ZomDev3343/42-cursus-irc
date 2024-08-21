#pragma once

# include "irc.hpp"
# include "IrcClient.hpp"

# define MAX_CLIENTS 10

class IrcClient;
class IrcServer
{
	private:
		int							port;
		std::string 				password;
		int							sockfd;
		int							epollfd;
		struct epoll_event			ev;
		struct epoll_event			events[MAX_CLIENTS];
		bool						stopped;
		std::map<int, IrcClient*>	clients;


		IrcServer();
	public:
		IrcServer(int &port, std::string &password);
		~IrcServer();

		bool	setupServer();
		void	serverLoop();
		void	stopServer();
};
