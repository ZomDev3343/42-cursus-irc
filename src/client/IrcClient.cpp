#include <iostream>
#include <cstring>
#include <cerrno>
#include "../../include/Irc.hpp"
#include "../../include/IrcClient.hpp"

IrcClient::IrcClient(int id, std::string host)
    : _id(id), _hostname(host), _channel(NULL)
{
}

IrcClient::~IrcClient()
{
}

int const &IrcClient::getId() const
{
    return this->_id;
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

std::string const &IrcClient::getLastMessage() const
{
    return this->_lastmsg;
}

bool IrcClient::appendMessagePart(std::string &msg_part)
{
    std::size_t endcharpos;
    this->_lastmsg.append(msg_part);
    endcharpos = this->_lastmsg.find('\r');
    if ((endcharpos != this->_lastmsg.npos && this->_lastmsg[endcharpos + 1] == '\n') ||
        this->_lastmsg.find('\n') != this->_lastmsg.npos)
    {
        return true;
    }
    return false;
}

void IrcClient::clearLastMessage()
{
    this->_lastmsg.clear();
}

void IrcClient::sendMessage(std::string message)
{
    std::string formattedMessage = message + "\r\n";
    std::cout << "Sending message to client " << this->_id << " : " << message << std::endl;
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

void IrcClient::setLastMessage(std::string newLastMessage)
{
    this->_lastmsg = newLastMessage;
}

void IrcClient::setChannel(Channel *channel)
{
    this->_channel = channel;
}

Channel *IrcClient::getChannel()
{
    return this->_channel;
}

bool IrcClient::is_enregister()
{
    return this->_enregister;
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
        buffer += msg;
}

std::string IrcClient::getBuffer() const {
        return buffer;
}

void IrcClient::clearBuffer()
{
	buffer.clear();
}
