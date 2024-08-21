#pragma once

class IrcClient
{
	private:
		int			_id;
		std::string _name;
		std::string _nickname;
		std::string _lastmsg;

		IrcClient();
	public:
		IrcClient(int id);
		~IrcClient();

		int const& getId() const;
		std::string const& getName() const;
		std::string const& getNickname() const;
		std::string const& getLastMessage() const;

		void	setName(std::string newName);
		void	setNickname(std::string newNickname);
		bool	appendMessagePart(std::string &msg_part);
		void	clearLastMessage();

};
