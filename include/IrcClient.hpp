#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <iostream>
#include <string>
#include "Channel.hpp"

# define MAX_PASSWORD_TRIES 3

class Channel;

class IrcClient
{
private:
    int _id;
    std::string _hostname;
    std::string _nickname;
    std::string _username;
    std::string _buffer;

    bool _logged;

    int _password_tries;

    Channel *_channel;

public:
    IrcClient(int id, std::string host);
    ~IrcClient();

    int const &getId() const;
    std::string const &getHostname() const;
    std::string const &getNickname() const;
    std::string getUsername() const;
    int const& getTries() const;
    Channel *getChannel();
    std::string getBuffer() const;

    void setUsername(std::string username);
    void setNickname(std::string newNickname);
    void setHostname(std::string newHost);
    void setChannel(Channel *channel);
    void setLogged();

    bool isLogged() const;

    void sendMessage(std::string message);
    void appendToBuffer(const std::string& msg);
    void clearBuffer();
    void incrementTries();
};

#endif
