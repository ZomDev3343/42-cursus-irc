#pragma once

# include "Irc.hpp"

class IrcServer;
class IrcClient;
class ACommand
{
	private:
		IrcServer	&_server;
		IrcClient	&_user;
		const std::string _name;
		ACommand();
	protected:
		IrcServer const& getServer() const;
		IrcClient const& getUser();
		std::string const& getName() const;
	public:
		ACommand(IrcServer &server, IrcClient &user, std::string cmdName);
		virtual ~ACommand();
		virtual void execute(std::string command) = 0;
};