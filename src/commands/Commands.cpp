#include "../../include/Irc.hpp"
#include "../../include/Commands.hpp"
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
    Channel *channel = server.getChannel(channelName);

    if (!channel)
    {
        channel = new Channel(channelName);
        server.getChannels().push_back(channel);
    }

    channel->addClient(&user);

    std::cout << "[IRC_REQUEST] :"
	    << user.getNickname()
	    << "!"
	    << user.getUsername()
	    << "@"
	    << user.getHostname()
	    << " JOIN :"
	    << channel->getName();

    channel->broadcast(":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN :" + channel->getName());
}

void Commands::nick_command(IrcServer &server, IrcClient &user, std::string command)
{
    (void)server;

    std::string nick = command.substr(command.find(" ") + 1);
    nick = nick.substr(0, nick.size() - 3);
    nick[nick.size()] = '\0';
    user.setNickname(nick);
}

void Commands::user_command(IrcServer &server, IrcClient &user, std::string command)
{
    (void)server;

    std::string name = command.substr(0, command.find(' '));
    std::vector<std::string>    argv;
    std::stringstream           line(command.substr(name.length(), command.length()));
    std::string                 buff;
    
    while (line >> buff)
                argv.push_back(buff);    
    user.setUsername(argv[0]);
}
