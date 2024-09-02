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
        return;

    if ((password_index = command.find(' ')) != std::string::npos)
    {
        if (command.find(' ', password_index + 1) != std::string::npos)
        {
            user.sendMessage(ERR_PASSWORD(user.getNickname()));
            return (std::cerr << "ERROR: Incorrect PASS command format" << std::endl, (void)0);
        }

        end_index = command.find_first_of("\r\n", password_index + 1);
        password = command.substr(password_index + 1, end_index - (password_index + 1));

        if (password == server.getPassword())
        {
            std::cout << "User " << user.getId() << " logged in succesfully!" << std::endl;
            user.setLogged();
            user.sendMessage("Good password!\r\n");
        }
        else
        {
            user.sendMessage(ERR_PASSWORD(user.getNickname()));
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
        {
            user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), target));
            return;
        }

        targetClient->sendMessage(":" + user.getNickname() + " PRIVMSG " + target + " :" + message + "\r\n");
    }
}

void Commands::join_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(3);
    Channel *channel;

	if (user.getNickname().empty() || user.getUsername().empty())
	{
		user.sendMessage(ERR_NOTREGISTERED(user.getNickname()));
		return ;
	}

    ss >> args[0];
    for (int i = 0; i < 3 && !ss.eof(); i++)
    {
        ss >> args[i];
        if (args[i] == "\n" || args[i] == "\r\n")
            args[i].clear();
        std::cout << args[i] << std::endl;
    }

    if (args[0][0] != '#')
    {
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), args[0]));
        return;
    }
    channel = server.getChannel(args[0]);

    if (!channel)
    {
        channel = new Channel(args[0]);
        server.addChannel(channel);
        channel->addOperator(&user);
    }
    if (channel->isInviteOnly() && !channel->isClientOperator(&user) && !channel->isInvited(&user))
    {
        user.sendMessage(ERR_INVITEONLYCHAN(user.getNickname(), channel->getName()));
        return;
    }
    else
        channel->removeInvited(&user);
    if (channel->getMaxClients() <= channel->getClients().size())
    {
        user.sendMessage(ERR_CHANNELISFULL(user.getNickname(), channel->getName()));
        return;
    }
    if (channel->getPassword() != "" && channel->getPassword() != args[1])
    {
        user.sendMessage(ERR_BADCHANNELKEY(user.getNickname(), channel->getName()));
        return;
    }
    channel->addClient(&user);
    std::cout << "[IRC_REQUEST] :"
              << user.getNickname()
              << " JOIN "
              << channel->getName() << std::endl;
    channel->broadcast(":" + user.getNickname() + " JOIN " + channel->getName() + "\r\n");
    for (std::vector<IrcClient *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); it++)
        if (*it != &user)
            user.sendMessage(":" + (*it)->getNickname() + " JOIN " + channel->getName() + "\r\n");
}

void Commands::nick_command(IrcServer &server, IrcClient &user, std::string command)
{
	if (!user.getNickname().empty())
	{
		user.sendMessage(ERR_ALREADY_REGISTERED(user.getNickname()));
		return ;
	}

	size_t		nick_pos = command.find(" ") + 1;
	size_t		end_pos = command.find_first_of("\r\n", nick_pos);
    std::string nick = command.substr(nick_pos, end_pos - nick_pos);

	if (!server.getClient(nick))
		user.setNickname(nick);
	else
		user.sendMessage(ERR_NICKNAMEINUSE(nick));
}

void Commands::user_command(IrcServer &server, IrcClient &user, std::string command)
{
    (void)server;

	if (user.getNickname().empty())
	{
		user.sendMessage(ERR_NONICKNAMEGIVEN);
		return ;
	}

	if (!user.getUsername().empty())
	{
		user.sendMessage(ERR_ALREADY_REGISTERED(user.getNickname()));
		return ;
	}

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
        if (channel->hasClientJoined(&user))
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
                            user.sendMessage(KICK_RPL(user.getNickname(), to_kick->getNickname(), channel->getName(), args[2]));
                            to_kick->sendMessage(KICK_RPL(user.getNickname(), to_kick->getNickname(), channel->getName(), args[2]));
                        }
                        else
                            user.sendMessage(ERR_NOTONCHANNEL(user.getNickname(), channel->getName()));
                    }
                    else
                        user.sendMessage(ERR_KICKOPERATOR(user.getNickname()));
                }
                else
                    user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), args[0]));
            }
            else
                user.sendMessage(ERR_CHANOPRIVSNEEDED(user.getNickname(), channel->getName()));
        }
        else
            user.sendMessage(ERR_NOTONCHANNEL(user.getNickname(), channel->getName()));
    }
    else
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel->getName()));
}

void Commands::topic_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(2);
    Channel *channel;

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
        if (channel->isClientOperator(&user) || !channel->isTopicOnlyOperator())
        {
            channel->setTopic(args[1]);
            channel->broadcast(":" + user.getNickname() + " TOPIC " + channel->getName() + " :" + args[1] + "\r\n");
        }
        else
            user.sendMessage(ERR_CHANOPRIVSNEEDED(user.getNickname(), channel->getName()));
    }
    else
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel->getName()));
}

void Commands::invite_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(2);
    Channel *channel;

    ss >> args[0];
    for (int i = 0; i < 2 && !ss.eof(); i++)
    {
        ss >> args[i];
        if (args[i] == "\n" || args[i] == "\r\n")
            args[i].clear();
        std::cout << args[i] << std::endl;
    }
    channel = server.getChannel(args[1]);
    if (channel)
    {
        if (channel->isClientOperator(&user))
        {
            IrcClient *to_invite = server.getClient(args[0]);
            if (to_invite)
            {
                if (!channel->hasClientJoined(to_invite) && !channel->isInvited(to_invite))
                {
                    channel->AddInvited(to_invite);
                    to_invite->sendMessage(":" + user.getNickname() + " INVITE " + to_invite->getNickname() + " " + channel->getName() + "\r\n");
                    user.sendMessage(":" + user.getNickname() + " INVITE " + to_invite->getNickname() + " " + channel->getName() + "\r\n");
                }
                else
                    user.sendMessage(ERR_USERONCHANNEL(user.getNickname(), to_invite->getNickname(), channel->getName()));
            }
            else
                user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), args[0]));
        }
        else
            user.sendMessage(ERR_CHANOPRIVSNEEDED(user.getNickname(), channel->getName()));
    }
    else
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel->getName()));
}

void operator_command(Channel *channel, IrcServer &server, IrcClient &user, std::vector<std::string> args)
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
            user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), args[2]));
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
            user.sendMessage(ERR_NOSUCHNICK(user.getNickname(), args[2]));
    }
}

void invite_mode_command(Channel *channel, IrcClient &user, std::vector<std::string> args)
{
    if (args[1] == "+i")
    {
        channel->setInviteOnly(true);
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " +i\r\n");
    }
    else if (args[1] == "-i")
    {
        channel->setInviteOnly(false);
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " -i\r\n");
    }
}

void limit_mode_command(Channel *channel, IrcClient &user, std::vector<std::string> args)
{
    if (args[1] == "+l")
    {
        channel->setMaxClients(std::atoi(args[2].c_str()));
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " +l " + args[2] + "\r\n");
    }
    else if (args[1] == "-l")
    {
        channel->setMaxClients(10);
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " -l\r\n");
    }
}

void pass_mode_command(Channel *channel, IrcClient &user, std::vector<std::string> args)
{
    if (args[1] == "+k")
    {
        channel->setPassword(args[2]);
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " +k " + args[2] + "\r\n");
    }
    else if (args[1] == "-k")
    {
        channel->setPassword("");
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " -k\r\n");
    }
}

void topic_mode_command(Channel *channel, IrcClient &user, std::vector<std::string> args)
{
    if (args[1] == "+t")
    {
        channel->setTopicOnlyOperator(true);
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " +t\r\n");
    }
    else if (args[1] == "-t")
    {
        channel->setTopicOnlyOperator(false);
        user.sendMessage(":" + user.getNickname() + " MODE " + channel->getName() + " -t\r\n");
    }
}

void Commands::mode_command(IrcServer &server, IrcClient &user, std::string command)
{
    std::stringstream ss(command);
    std::vector<std::string> args(3);
    Channel *channel;

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
            if (args[1] == "+o" || args[1] == "-o")
                operator_command(channel, server, user, args);
            else if (args[1] == "+i" || args[1] == "-i")
                invite_mode_command(channel, user, args);
            else if (args[1] == "+l" || args[1] == "-l")
                limit_mode_command(channel, user, args);
            else if (args[1] == "+k" || args[1] == "-k")
                pass_mode_command(channel, user, args);
            else if (args[1] == "+t" || args[1] == "-t")
                topic_mode_command(channel, user, args);
            else
                user.sendMessage("Unknown mode!\r\n");
        }
        else
            user.sendMessage(ERR_CHANOPRIVSNEEDED(user.getNickname(), channel->getName()));
    }
    else
        user.sendMessage(ERR_NOSUCHCHANNEL(user.getNickname(), channel->getName()));
}
