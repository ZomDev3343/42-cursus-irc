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
    std::cout << "[user : " << user.getId() << "] joined the channel: " << channel->getName() << std::endl;

    channel->broadcast(":" + user.getNickname() + "!" + user.getUsername() + "ubuntu@" + user.getHostname() + " JOIN :" + channel->getName());
}

void Commands::nick_command(IrcServer &server, IrcClient &user, std::string command)
{
    (void)server;

    std::string nick = command.substr(command.find(" ") + 1);
    user.setNickname(nick);
}

void Commands::user_command(IrcServer &server, IrcClient &user, std::string command)
{
    (void)server;

    std::cout << "User command starting..." << std::endl;
    // user.setUsername(username);
}
