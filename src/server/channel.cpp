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
  for (std::vector<IrcClient*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
  {
    (*it)->sendMessage(message);
  }
}

void Channel::addClient(IrcClient* client)
{
  this->_clients.push_back(client);
  
}

void Channel::printClient()
{
	std::cout << "all client" << std::endl;
	for (std::vector<IrcClient*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
  	{
		std::cout << (*it)->getUsername() << std::endl;
  	}
}
