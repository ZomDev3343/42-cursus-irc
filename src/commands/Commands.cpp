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
    std::cout << "[user : " << user.getId() << "] join the channel :" << channel->getName() << std::endl;
    channel->printClient();

    std::cout << "user info : " << user.getId() << " " << user.getNickname() << " " << user.getUsername() << " " << user.getRealname() << std::endl;
    channel->broadcast(":ubuntu!ubuntu@" + user.getHost() + " JOIN :" + channel->getName());
}

void Commands::nick_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::string nick = command.substr(command.find(" ") + 1, command.size());
    user.setNick(nick);
    std::cout << "[user : " << user.getId() << "] change his nickname to : " << nick << std::endl;
}

void Commands::user_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::vector<std::string>    args;
    std::stringstream           line(syntax.substr(name.length(), syntax.length()));
    std::string                 buf;

    Command *cmd = _commands.at(name);

    while (line >> buf)
        args.push_back(buf);

    user.setUsername(args[0]);
    user.setRealname(args[3]);
}
