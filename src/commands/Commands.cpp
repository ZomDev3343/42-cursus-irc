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
    channel->printClient();

    std::cout << "user info : " << user.getId() << " "
              << user.getNickname() << " "
              << user.getUsername() << " "
              << user.getRealname() << std::endl;
    channel->broadcast(":ubuntu!ubuntu@" + user.getHostname() + " JOIN :" + channel->getName());
}

void Commands::nick_command(IrcServer &server, IrcClient &user, std::string command)
{
	(void) server;
	std::cout << command << "lala" << std::endl << std::endl;
    std::string nick = command.substr(command.find(" ") + 1);
    user.setNickname(nick);
    std::cout << "[user : " << user.getId() << "] changed their nickname to: " << nick << std::endl;
}

void Commands::user_command(IrcServer &server, IrcClient &user, std::string command)
{
    (void) server;
    std::vector<std::string> args;
    std::stringstream line(command);
    std::string buf;

    while (line >> buf)
        args.push_back(buf);

    if (args.size() < 4) {
        std::cerr << "Not enough arguments for USER command" << std::endl;
        return;
    }

    user.setUsername(args[0]);
    user.setRealname(args[3]);
}

