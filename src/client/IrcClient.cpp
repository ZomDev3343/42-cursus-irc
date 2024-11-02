#include <iostream>
#include <cstring>
#include <cerrno>
#include "../../include/Irc.hpp"
#include "../../include/IrcClient.hpp"

IrcClient::IrcClient(int id, std::string host)
    : _id(id), _hostname(host), _channel(NULL)
{
    this->_logged = false;
    this->_password_tries = 0;
    this->setIsBot(false);
}

IrcClient::~IrcClient()
{
}

int const &IrcClient::getId() const
{
    return this->_id;
}

bool IrcClient::isBot() const {
    return this->_is_bot;
}

std::string const &IrcClient::getHostname() const
{
    return this->_hostname;
}

std::string const &IrcClient::getNickname() const
{
    return this->_nickname;
}

void IrcClient::setNickname(std::string newNickname)
{
    if (this->_nickname.empty())
    {
        this->_nickname = newNickname;
    }
}

void IrcClient::setIsBot(bool value) {
    this->_is_bot = value;
}

void IrcClient::sendMessage(std::string message)
{
    std::string formattedMessage = message;
    if (send(this->_id, formattedMessage.c_str(), formattedMessage.size(), 0) == -1)
    {
        std::cerr << "Error while sending message to client " << this->_id
                  << " : " << strerror(errno) << std::endl;
    }
}

void IrcClient::setHostname(std::string newHost)
{
    this->_hostname = newHost;
}

void IrcClient::setChannel(Channel *channel)
{
    this->_channel = channel;
}

Channel *IrcClient::getChannel()
{
    return this->_channel;
}

bool IrcClient::isLogged() const
{
    return this->_logged;
}

void IrcClient::setUsername(std::string username)
{
    this->_username = username;
}

std::string IrcClient::getUsername() const
{
    return this->_username;
}

void IrcClient::appendToBuffer(const std::string& msg) {
        _buffer += msg;
}

std::string IrcClient::getBuffer() const {
        return _buffer;
}

void IrcClient::clearBuffer()
{
	_buffer.clear();
}

void IrcClient::setLogged()
{
    this->_logged = true;
}

int const& IrcClient::getTries() const
{
    return this->_password_tries;
}

void IrcClient::incrementTries()
{
    this->_password_tries++;
}
