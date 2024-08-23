#pragma once

class IrcClient
{
	private:
		int			_id;
		int			_rankId;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _lastmsg;
		std::string _hostname;

		IrcClient();
	public:
		IrcClient(int id, std::string host);
		~IrcClient();

		int const& getId() const;
		std::string const& getName() const;
		std::string const& getHost() const;
		std::string const& getNickname() const;
		std::string const& getLastMessage() const;

		void	setName(std::string newName);
		void	setNickname(std::string newNickname);
		void	setLastMessage(std::string newLastMessage);
		void	setHost(std::string newHost);
		bool	appendMessagePart(std::string &msg_part);
		void  sendMessage(std::string message);
		void	clearLastMessage();

};
