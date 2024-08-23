#pragma once

class IrcClient
{
	private:
		int			_id;
		int			_rankId;
		std::string _name;
		std::string _nickname;
		std::string _lastmsg;
		std::string _host;

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
		bool	appendMessagePart(std::string &msg_part);
		void  sendMessage(std::string message);
		void	clearLastMessage();

};
