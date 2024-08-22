#pragma once

class IrcServer;
class IrcClient;
class Commands
{
	private:
		Commands();
	public:
		static void	pass_command(IrcServer &server,
			IrcClient &user, std::string command);
		

};
