# include "../../include/ACommand.hpp"

ACommand::ACommand(IrcServer &server, IrcClient &user, std::string cmdName)
	: _server(server), _user(user), _name(cmdName)
{
}

ACommand::~ACommand()
{
}

IrcServer const & ACommand::getServer() const
{
	return this->_server;
}

IrcClient const &ACommand::getUser()
{
	return this->_user;
}

std::string const & ACommand::getName() const
{
	return this->_name;
}


