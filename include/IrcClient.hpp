#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <iostream>
#include <string>
#include "Channel.hpp" // Assurez-vous d'inclure l'en-tête de Channel

class Channel; // Déclaration anticipée

class IrcClient {
private:
    int _id;
    std::string _hostname;
    int _rankId;          // Déplacez _rankId après _hostname
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _lastmsg;
    
    bool enregister;
    Channel* _channel;

public:
    IrcClient(int id, std::string host);
    ~IrcClient();

    int const& getId() const;
    std::string const& getHostname() const;
    std::string const& getNickname() const;
    std::string const& getLastMessage() const;
    std::string getUsername() const;
    std::string getRealname() const;

    void setUsername(std::string username);
    void setRealname(std::string realname);
    void setNickname(std::string newNickname);
    void setLastMessage(std::string newLastMessage);
    void setHostname(std::string newHost);
    void setRankId(int newRankId);
    int getRankId();
    void setChannel(Channel* channel);
    Channel* getChannel();
    bool appendMessagePart(std::string& msg_part);
    void sendMessage(std::string message);
    void clearLastMessage();
};

#endif
