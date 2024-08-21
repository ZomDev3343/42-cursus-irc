#pragma once

# include "irc.hpp"

class IrcClient
{
	private:
		int			id;
		std::string name;
		std::string nickname;

	public:
		IrcClient();
		IrcClient(int id);
		~IrcClient();

};
