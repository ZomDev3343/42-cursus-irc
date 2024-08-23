# include "../../include/Irc.hpp"
# include "../../include/IrcClient.hpp"

IrcClient::IrcClient(int id)
{
	this->_id = id;
}

IrcClient::~IrcClient()
{

}

int const &IrcClient::getId() const
{
	return this->_id;
}

std::string const &IrcClient::getName() const
{
	return this->_name;
}

std::string const &IrcClient::getNickname() const
{
	return this->_nickname;
}

void IrcClient::setName(std::string newName)
{
	if (this->_name.empty())
		this->_name = newName;
}

void IrcClient::setNickname(std::string newNickname)
{
	if (this->_nickname.empty())
		this->_nickname = newNickname;
}

std::string const& IrcClient::getLastMessage() const
{
	return this->_lastmsg;
}

/**
 * @brief Makes the entire commands based on every parts it gets until a '\\n' is found
 * @return bool Does the entire command is correctly ended with a '\\n' ?
 */
bool IrcClient::appendMessagePart(std::string &msg_part)
{
	std::size_t	endcharpos;
	this->_lastmsg.append(msg_part);
	endcharpos = this->_lastmsg.find('\r');
	if ((endcharpos != this->_lastmsg.npos && this->_lastmsg[endcharpos + 1] == '\n')
		|| this->_lastmsg.find('\n') != this->_lastmsg.npos)
		return true;
	return false;
}

void	IrcClient::clearLastMessage()
{
	this->_lastmsg.clear();
}

void IrcClient::sendMessage(std::string message)
{
	std::cout << "Sending message to client " << this->_id << " : " << message << std::endl;
	if (send(this->_id, message.c_str(), message.size(), 0) == -1)
	{
		std::cerr << "Error while sending message to client " << this->_id << " : " << strerror(errno) << std::endl;
	}
}	
