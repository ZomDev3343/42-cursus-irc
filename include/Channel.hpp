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
  Channel();

public:
  Channel(std::string name);
  ~Channel();

  void broadcast(std::string message);
  void addClient(IrcClient *client);
  void removeClient(IrcClient *client);
  void addOperator(IrcClient *client);
  void removeOperator(IrcClient *client);
  void printClient();

  std::string getName();
};

#endif
