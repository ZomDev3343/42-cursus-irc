#pragma once

# define MAX_CLIENTS 10

class IrcClient;
class IrcServer;
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

		IrcServer();

		void	interpret_message(int user_id, char buffer[256], int const& msglen);

	public:
		IrcServer(int &port, std::string &password);
		~IrcServer();

		std::string const& getPassword() const;

		bool	setupServer();
		void	serverLoop();
		void	stopServer();

		void	close_client_connection(int user_id, std::string reason = "");
};
