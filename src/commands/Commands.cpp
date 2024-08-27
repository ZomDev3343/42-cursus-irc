#include "../../include/Irc.hpp"
#include "../../include/Commands.hpp"
#include <sstream>

void Commands::pass_command(IrcServer &server, IrcClient &user, std::string command)
{
	std::string	password;
	size_t		password_index;
	size_t		end_index;
	if (user.isLogged())
	{
		std::cout << "User " << user.getId() << " is already logged in!" << std::endl;
		user.sendMessage("You are already logged in!");
		return ;
	}
	if ((password_index = command.find(' ')) != std::string::npos)
	{
		if (command.find(' ', password_index + 1) != std::string::npos)
			return (std::cout << "Error: Incorrect PASS command format" << std::endl, (void) 0);
		end_index = command.find_first_of("\r\n", password_index + 1);
		password = command.substr(password_index + 1, end_index - (password_index + 1));
		std::cout << "Password given with PASS command : " << password << std::endl;
		if (password == server.getPassword())
		{
			std::cout << "User " << user.getId() << " logged in succesfully!" << std::endl;
			user.setLogged();
		}
		else
			server.close_client_connection(user.getId(), "Bad Password!");
	}
	else
		server.close_client_connection(user.getId(), "No password given!");
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
