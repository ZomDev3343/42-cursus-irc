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

void Commands::part_command(IrcServer &server, IrcClient &user, std::string command)
{
    size_t spacePos = command.find(" ");
    if (spacePos == std::string::npos)
        return;
    std::string channelName = command.substr(spacePos + 1);
    if (channelName.empty() || channelName[0] != '#')
        return;
    channelName = channelName.substr(1);
    channelName = channelName.substr(0, channelName.find_first_of(" \r\n"));
    Channel *channel = server.getChannel(channelName);
    if (!channel)
        return;
    std::cout << "[IRC_REQUEST] :"
              << user.getNickname()
              << "!"
              << user.getUsername()
              << "@"
              << user.getHostname()
              << " PART :"
              << channel->getName() << std::endl;
    channel->removeClient(&user);
    channel->broadcast(":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " PART :#" + channel->getName() + "\r\n");
}

void Commands::privmsg_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::string target = command.substr(command.find(" ") + 1, command.find(" ", command.find(" ") + 1) - command.find(" ") - 1);
    std::string message = command.substr(command.find(" ", command.find(" ") + 1) + 1, command.size());

    if (target[0] == '#')
    {
        Channel *channel = server.getChannel(target);

        if (!channel)
            return;

        channel->broadcast(":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " PRIVMSG " + channel->getName() + " :" + message);
    }
    else
    {
        IrcClient *targetClient = server.getClient(target);

        if (!targetClient)
            return;

        targetClient->sendMessage(":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " PRIVMSG " + target + " :" + message);
    }
}

void Commands::join_command(IrcServer &server, IrcClient &user, std::string command)
{
    size_t spacePos = command.find(" ");
    if (spacePos == std::string::npos)
        return;
    std::string channelName = command.substr(spacePos + 1);
    if (channelName.empty() || channelName[0] != '#')
        return;
    channelName = channelName.substr(1);
    channelName = channelName.substr(0, channelName.find_first_of(" \r\n"));
    Channel *channel = server.getChannel(channelName);
    std::cout << "channel name join : " << channelName << std::endl;

    if (!channel)
    {
        channel = new Channel(channelName);
        server.addChannel(channel);
        channel->addOperator(&user);
    }
    channel->addClient(&user);
    std::cout << "[IRC_REQUEST] :"
              << user.getNickname()
              << "!"
              << user.getUsername()
              << "@"
              << user.getHostname()
              << " JOIN :"
              << channel->getName() << std::endl;
    channel->broadcast(":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN :" + channel->getName() + "\r\n");
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
    std::vector<std::string> argv;
    std::stringstream line(command.substr(name.length(), command.length()));
    std::string buff;

    while (line >> buff)
        argv.push_back(buff);
    user.setUsername(argv[0]);
}
