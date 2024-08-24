#include "../../include/Channel.hpp"

Channel::Channel(std::string name)
{
  this->_name = name;
  this->_maxClients = 10;
}

Channel::~Channel()
{
}

std::string Channel::getName()
{
  return this->_name;
}

void Channel::broadcast(std::string message)
{
  for (std::vector<IrcClient *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
  {
    (*it)->sendMessage(message);
  }
}

void Channel::broadcast(std::string message, IrcClient *client)
{
  for (std::vector<IrcClient *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
  {
    if ((*it) != client)
      (*it)->sendMessage(message);
  }
}

void Channel::addClient(IrcClient *client)
{
  this->_clients.push_back(client);
}

void Channel::printClient()
{
  std::cout << "all client" << std::endl;
  for (std::vector<IrcClient *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
  {
    std::cout << (*it)->getUsername() << std::endl;
  }
}

void Channel::removeClient(IrcClient *client)
{
  this->_clients.erase(std::remove(this->_clients.begin(), this->_clients.end(), client), this->_clients.end());
}

void Channel::addOperator(IrcClient *client)
{
  this->_operators.push_back(client);
}

void Channel::removeOperator(IrcClient *client)
{
  this->_operators.erase(std::remove(this->_operators.begin(), this->_operators.end(), client), this->_operators.end());
}
