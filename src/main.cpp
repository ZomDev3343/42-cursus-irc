# include "../include/irc.hpp"

int main(int ac, char **av)
{
	int			port;
	std::string	password;

	(void)port;

	if (ac == 3)
	{
		port = std::atoi(av[1]);
		password = av[2];
	}
	else
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
	return 0;
}