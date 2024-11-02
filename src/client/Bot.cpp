#include "../../include/Bot.hpp"

Bot::Bot(const std::string& host, IrcServer* server)
    : IrcClient(-1, host), _server(server)
{
    this->setNickname("Bot");
    this->setUsername("Bot");
    this->setLogged();
}

Bot::~Bot()
{
}

void Bot::sendHelloMessage(const std::string& channelName, const std::string& userName)
{
    std::string message = "PRIVMSG " + channelName + " :Bonjour " + userName + " !\r\n";
    this->sendMessage(message);
}

void Bot::sendMessage(std::string message)
{
    size_t pos = message.find("PRIVMSG ");
    if (pos == std::string::npos)
        return;

    pos += 8;
    size_t end = message.find(" :", pos);
    if (end == std::string::npos)
        return;

    std::string targetName = message.substr(pos, end - pos);
    std::string actualMessage = message.substr(end + 2); // Skip " :"

    _server->sendMessageFromBot(this, targetName, actualMessage);
}
