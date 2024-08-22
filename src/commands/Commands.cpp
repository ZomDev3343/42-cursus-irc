# include "../../include/Irc.hpp"
# include "../../include/Commands.hpp"

void Commands::pass_command(IrcServer &server, IrcClient &user, std::string command)
{
	(void)server;
	(void)user;
	(void)command;
	std::cout << "Pass Command starting..." << std::endl;
	std::cout << "Pass Command ended" << std::endl;
}