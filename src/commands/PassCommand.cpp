# include "../../include/ACommand.hpp"
# include "../../include/Commands.hpp"

PassCommand::PassCommand(IrcServer &server, IrcClient &user)
	: ACommand::ACommand(server, user, "PASS")
{

}
PassCommand::~PassCommand()
{

}

void PassCommand::execute(std::string command)
{
	(void)command;
	std::cout << "Rentre le mot de passe batar!" << std::endl;
}