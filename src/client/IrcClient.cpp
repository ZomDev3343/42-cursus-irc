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
