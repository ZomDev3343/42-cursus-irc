#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "IrcClient.hpp"
#include "IrcServer.hpp"

class Channel
{
private:
  std::string _name;
  std::vector<IrcClient *> _clients;
  std::vector<IrcClient *> _operators;

  size_t _maxClients;
  std::string _topic;
  bool inviteOnly;
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
  void setTopic(std::string topic);
  void removeOperator(IrcClient *client);
  void printClient();
  bool isClientOperator(IrcClient *client);
  bool hasClientJoined(IrcClient *client);

  std::string getName();
};

#endif
