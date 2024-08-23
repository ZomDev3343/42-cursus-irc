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

    std::cout << "[user : " << user.getId() << "] join the channel :" << channel->getName() << std::endl;
    std::stringstream ss;
    ss << user.getId();
    std::string userIdStr = ss.str();
    channel->printClient();
    channel->broadcast("User " + userIdStr + " joined the channel");
}
