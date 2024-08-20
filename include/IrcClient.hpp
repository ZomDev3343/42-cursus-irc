#pragma once

# include "irc.hpp"

class IrcClient
{
	private:
		int			id;
		std::string name;
		std::string nickname;

		IrcClient();
	public:
		IrcClient(int id);
		~IrcClient();

};
