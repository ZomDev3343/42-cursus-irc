#pragma once

# include "irc.hpp"

# define MAX_CLIENTS 10

class IrcServer
{
	private:
		int					port;
		std::string 		password;
		int					sockfd;
		int					epollfd;
		struct epoll_event	ev;
		struct epoll_event	events[MAX_CLIENTS];
		bool				stopped;


		IrcServer();
	public:
		IrcServer(int &port, std::string &password);
		~IrcServer();

		bool	setupServer();
		void	serverLoop();
		void	stopServer();
};
