#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <iostream>
#include <string>
#include "Channel.hpp"

class Channel; 

class IrcClient {
private:
    int _id;
    std::string _hostname;
    std::string _nickname;
    std::string _username;
    std::string _lastmsg;
    
    bool _enregister;

    Channel* _channel;

public:
    IrcClient(int id, std::string host);
    ~IrcClient();

    int const& getId() const;
    std::string const& getHostname() const;
    std::string const& getNickname() const;
    std::string const& getLastMessage() const;
    std::string getUsername() const;

    void setUsername(std::string username);
    void setNickname(std::string newNickname);
    void setLastMessage(std::string newLastMessage);
    void setHostname(std::string newHost);
    void setChannel(Channel* channel);
    Channel* getChannel();
    bool is_enregister();
    bool appendMessagePart(std::string& msg_part);
    void sendMessage(std::string message);
    void clearLastMessage();
};

#endif
