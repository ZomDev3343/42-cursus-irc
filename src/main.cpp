# include "../include/Irc.hpp"
# include "../include/IrcServer.hpp"

int	g_stopSignal = 0;

static void onStopSignal(int signal)
{
	if (signal == SIGINT)
		g_stopSignal = 1;
}

int main(int ac, char **av)
{
	int			port;
	std::string	password;

	if (ac == 3)
	{
		port = std::atoi(av[1]);
		password = av[2];
		IrcServer server(port, password);
		if (server.setupServer())
		{
			signal(SIGINT, onStopSignal);
			server.serverLoop();
		}
		else
			std::cerr << "Error: Can't setup server properly, check other errors!" << std::endl;
	}
	else
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
	return 0;
}
