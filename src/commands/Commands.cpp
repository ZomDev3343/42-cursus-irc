#include "../../include/Irc.hpp"
#include "../../include/Commands.hpp"
#include "../../include/RPL.hpp"
#include <sstream>

void Commands::pass_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::string password;
    size_t password_index;
    size_t end_index;
    if (user.isLogged())
    {
        user.sendMessage("You are already logged in!\r\n");
        return;
    }
    if ((password_index = command.find(' ')) != std::string::npos)
    {
        if (command.find(' ', password_index + 1) != std::string::npos)
        {
            user.sendMessage("Wrong command format -> PASS <password>\r\n");
            return (std::cerr << "ERROR: Incorrect PASS command format" << std::endl, (void)0);
        }

        end_index = command.find_first_of("\r\n", password_index + 1);
        password = command.substr(password_index + 1, end_index - (password_index + 1));

        if (password == server.getPassword())
        {
            std::cout << "User " << user.getId() << " logged in succesfully!" << std::endl;
            user.setLogged();
            user.sendMessage("You successfully logged in!\r\n");
        }
        else
        {
            user.sendMessage("Bad Password!\r\n");
            user.incrementTries();
            if (user.getTries() >= MAX_PASSWORD_TRIES)
            {
                user.sendMessage("You typed too many wrong passwords!\r\n");
                server.close_client_connection(user.getId(), "Too much password tries!");
            }
        }
    }
    else
        user.sendMessage("No password given!\r\n");
}

void Commands::part_command(IrcServer &server, IrcClient &user, std::string command)
{
    size_t spacePos = command.find(" ");
    if (spacePos == std::string::npos)
        return;
    std::string channelName = command.substr(spacePos + 1);
    if (channelName.empty() || channelName[0] != '#')
        return;
    channelName = channelName.substr(0, channelName.find_first_of(" \r\n"));
    Channel *channel = server.getChannel(channelName);
    if (!channel)
        return;
    std::cout << "[IRC_REQUEST] :"
              << user.getNickname()
              << " PART "
              << channel->getName() << std::endl;
    channel->broadcast(":" + user.getNickname() + " PART " + channelName + "\r\n");
    channel->removeClient(&user);
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

        channel->broadcast(":" + user.getNickname() + " PRIVMSG " + channel->getName() + " :" + message + "\r\n", &user);
    }
    else
    {
        IrcClient *targetClient = server.getClient(target);

        if (!targetClient)
            return;

        targetClient->sendMessage(":" + user.getNickname() + " PRIVMSG " + target + " :" + message + "\r\n");
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
    channelName = channelName.substr(0, channelName.find_first_of(" \r\n"));
    Channel *channel = server.getChannel(channelName);

    if (!channel)
    {
        channel = new Channel(channelName);
        server.addChannel(channel);
        channel->addOperator(&user);
    }
    channel->addClient(&user);
    std::cout << "[IRC_REQUEST] :"
              << user.getNickname()
              << " JOIN "
              << channel->getName() << std::endl;
    channel->broadcast(":" + user.getNickname() + " JOIN " + channel->getName() + "\r\n");
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

void Commands::kick_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(3);
    Channel *channel;

    (void)server;
    (void)user;
    ss >> args[0];
    for (int i = 0; i < 3 && !ss.eof(); i++)
    {
        ss >> args[i];
        if (args[i] == "\n" || args[i] == "\r\n")
            args[i].clear();
        std::cout << args[i] << std::endl;
    }

    channel = server.getChannel(args[0]);
    if (channel)
    {
        if (channel->isClientOperator(&user))
        {
            IrcClient *to_kick = server.getClient(args[1]);
            if (to_kick)
            {
                if (!channel->isClientOperator(to_kick))
                {
                    if (channel->hasClientJoined(to_kick))
                    {
                        channel->removeClient(to_kick);
                        user.sendMessage(KICK_RPL(user.getNickname(), channel->getName(), args[2]));
                    }
                    else
                        user.sendMessage("This user hasn't joined the channel!\r\n");
                }
                else
                    user.sendMessage("You can't kick an operator!\r\n");
            }
            else
                user.sendMessage("Unknown user nickname!\r\n");
        }
        else
            user.sendMessage("You don't have the rights to kick someone on this channel!\r\n");
    }
    else
        user.sendMessage("Unknown channel!\r\n");
}

void Commands::topic_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(2);
    Channel *channel;

    (void)server;
    (void)user;
    ss >> args[0];
    for (int i = 0; i < 2 && !ss.eof(); i++)
    {
        ss >> args[i];
        if (args[i] == "\n" || args[i] == "\r\n")
            args[i].clear();
        std::cout << args[i] << std::endl;
    }

    channel = server.getChannel(args[0]);
    if (channel)
    {
        if (channel->isClientOperator(&user))
        {
            channel->setTopic(args[1]);
            user.sendMessage(":" + user.getNickname() + " TOPIC " + channel->getName() + " :" + args[1] + "\r\n");
        }
        else
            user.sendMessage("You don't have the rights to change the topic on this channel!\r\n");
    }
    else
        user.sendMessage("Unknown channel!\r\n");
}

void Commands::invite_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(2);
    Channel *channel;

    (void)server;
    (void)user;
    ss >> args[0];
    for (int i = 0; i < 2 && !ss.eof(); i++)
    {
        ss >> args[i];
        if (args[i] == "\n" || args[i] == "\r\n")
            args[i].clear();
        std::cout << args[i] << std::endl;
    }

    channel = server.getChannel(args[0]);
    if (channel)
    {
        if (channel->isClientOperator(&user))
        {
            IrcClient *to_invite = server.getClient(args[1]);
            if (to_invite)
            {
                if (!channel->hasClientJoined(to_invite))
                {
                    to_invite->sendMessage(":" + user.getNickname() + " INVITE " + to_invite->getNickname() + " " + channel->getName() + "\r\n");
                    user.sendMessage(":" + user.getNickname() + " INVITE " + to_invite->getNickname() + " " + channel->getName() + "\r\n");
                }
                else
                    user.sendMessage("This user has already joined the channel!\r\n");
            }
            else
                user.sendMessage("Unknown user nickname!\r\n");
        }
        else
            user.sendMessage("You don't have the rights to invite someone on this channel!\r\n");
    }
    else
        user.sendMessage("Unknown channel!\r\n");
}

void Commands::mode_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(3);
    Channel *channel;

    (void)server;
    (void)user;
    ss >> args[0];
    for (int i = 0; i < 3 && !ss.eof(); i++)
    {
        ss >> args[i];
        if (args[i] == "\n" || args[i] == "\r\n")
            args[i].clear();
        std::cout << args[i] << std::endl;
    }

    channel = server.getChannel(args[0]);
    if (channel)
    {
        if (channel->isClientOperator(&user))
        {
            if (args[1] == "+o")
            {
                IrcClient *to_op = server.getClient(args[2]);
                if (to_op)
                {
                    if (!channel->isClientOperator(to_op))
                    {
                        channel->addOperator(to_op);
                        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " +o " + to_op->getNickname() + "\r\n");
                    }
                    else
                        user.sendMessage("This user is already an operator!\r\n");
                }
                else
                    user.sendMessage("Unknown user nickname!\r\n");
            }
            else if (args[1] == "-o")
            {
                IrcClient *to_deop = server.getClient(args[2]);
                if (to_deop)
                {
                    if (channel->isClientOperator(to_deop))
                    {
                        channel->removeOperator(to_deop);
                        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " -o " + to_deop->getNickname() + "\r\n");
                    }
                    else
                        user.sendMessage("This user is not an operator!\r\n");
                }
                else
                    user.sendMessage("Unknown user nickname!\r\n");
            }
            else
                user.sendMessage("Unknown mode!\r\n");
        }
        else
            user.sendMessage("You don't have the rights to change the mode on this channel!\r\n");
    }
    else
        user.sendMessage("Unknown channel!\r\n");
}
