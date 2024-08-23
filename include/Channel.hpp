#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include "IrcClient.hpp"
# include "IrcServer.hpp"

class Channel
{
  private:
    std::string					_name;
    std::vector<IrcClient*>		_clients;
    
    size_t _maxClients;
    Channel();
  public:
    Channel(std::string name);
    ~Channel();

    void broadcast(std::string message);
    void addClient(IrcClient* client);

    std::string getName();

};

#endif
