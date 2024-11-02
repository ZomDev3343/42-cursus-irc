#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include "IrcClient.hpp"
#include "IrcServer.hpp"

class Bot : public IrcClient
{
private:
    Bot();
	IrcServer* _server;

public:
    Bot(const std::string& host, IrcServer* server);
    ~Bot();

    void sendHelloMessage(const std::string& channelName, const std::string& userName);
    void sendMessage(std::string message);
};

#endif
