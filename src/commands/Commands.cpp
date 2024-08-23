# include "../../include/Irc.hpp"
# include "../../include/IrcServer.hpp"
# include "../../include/IrcClient.hpp"
# include "../../include/Commands.hpp"

void Commands::pass_command(IrcServer &server, IrcClient &user, std::string command)
{
	std::vector<std::string>	parts;

	std::cout << "Pass Command starting..." << std::endl;
	
	parts = split_string(command);
	
	std::cout << "Command Parts" << std::endl;
	for (std::vector<std::string>::iterator it = parts.begin();
			it != parts.end(); it++)
		std::cout << *it << std::endl;
	
	std::string					&given_password = parts[1];
	if (parts.size() != 2 || given_password != server.getPassword())
		server.close_client_connection(user.getId(), ERROR_WRONG_PASSWORD);
	else
		user.setLogged();

	std::cout << "Pass Command ended" << std::endl;
}