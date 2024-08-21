#pragma once

class PassCommand : public ACommand {
	public:
		PassCommand(IrcServer &server, IrcClient &user);
		~PassCommand();
		virtual void execute(std::string command);
};
