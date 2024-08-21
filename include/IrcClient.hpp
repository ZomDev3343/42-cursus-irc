#pragma once

# include "irc.hpp"

class IrcClient
{
	private:
		int			_id;
		std::string _name;
		std::string _nickname;

		IrcClient();
	public:
		IrcClient(int id);
		~IrcClient();

		int const& getId() const;
		std::string const& getName() const;
		std::string const& getNickname() const;

		void	setName(std::string newName);
		void	setNickname(std::string newNickname);

};
