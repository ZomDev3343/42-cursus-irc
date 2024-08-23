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

void Commands::join_command(IrcServer &server, IrcClient &user, std::string command)
{
	(void)user;
	(void)command;

	Channel *Channel = server.getChannel(command.substr(command.find(" ") + 1, command.size()));
	if (!Channel)
	{
		Channel *Channel = new Channel(command.substr(command.find(" ") + 1, command.size()));
		server.getChannels().pushback(Channel);
	}
	
	std::cout << "[user : " << user.getId() << "] join the channel :" << Channel.getName() << std::endl;
}