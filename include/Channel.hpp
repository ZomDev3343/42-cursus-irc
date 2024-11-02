#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "IrcServer.hpp"
#include "IrcClient.hpp"

class Channel
{
private:
  std::string _name;
  std::vector<IrcClient *> _clients;
  std::vector<IrcClient *> _operators;
  std::vector<IrcClient *> _invited;

  size_t _maxClients;
  std::string _topic;
  bool _topicOnlyOperator;
  bool _inviteOnly;
  std::string _password;
  Channel();

public:
  Channel(std::string name);
  ~Channel();

  void broadcast(std::string message);
  void broadcast(std::string message, IrcClient *client);
  void addClient(IrcClient *client);
  void removeClient(IrcClient *client);
  void addOperator(IrcClient *client);
  void setInviteOnly(bool inviteOnly);
  void setTopicOnlyOperator(bool topicOnlyOperator);
  bool isTopicOnlyOperator();
  bool isInviteOnly();
  void AddInvited(IrcClient *client);
  void removeInvited(IrcClient *client);
  void setMaxClients(int maxClients);
  bool isInvited(IrcClient *client);
  void setPassword(std::string password);
  std::string getPassword();
  size_t getMaxClients();
  void setTopic(std::string topic);
  void removeOperator(IrcClient *client);
  void printClient();
  bool isClientOperator(IrcClient *client);
  bool hasClientJoined(IrcClient *client);
  std::vector<IrcClient *> &getClients();

  std::string getName();
};

#endif
