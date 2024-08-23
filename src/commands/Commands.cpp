# include "../../include/Irc.hpp"
# include "../../include/Commands.hpp"
#include <sstream>

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
    std::string channelName = command.substr(command.find(" ") + 1, command.size());
    Channel* channel = server.getChannel(channelName);

    if (!channel)
    {
        channel = new Channel(channelName);
        server.getChannels().push_back(channel);
    }

    channel->addClient(&user);
    std::cout << "[user : " << user.getId() << "] joined the channel: " << channel->getName() << std::endl;

    channel->broadcast(":" + user.getNickname() + "!" + user.getUsername() + "ubuntu@" + user.getHostname() + " JOIN :" + channel->getName());
}

void Commands::nick_command(IrcServer &server, IrcClient &user, std::string command)
{
	(void) server;

    if (!user.is_enregister())
    {
        std::vector<std::string> args;
        std::stringstream line(command);
        std::string buf;

        while (line >> buf)
            args.push_back(buf);
	
	std::cout << args[1] << " jriuejgei " << args[3] << std::endl;
        user.setNickname(args[1]);
        user.setUsername(args[3]);
    }
    else
    {
        std::string nick = command.substr(command.find(" ") + 1);
        user.setNickname(nick);
        std::cout << "[user : " << user.getId() << "] changed their nickname to: " << nick << std::endl;
    }
}

