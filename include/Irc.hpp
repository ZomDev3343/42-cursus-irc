#pragma once

# include <iostream>
# include <cstring>
# include <csignal>
# include <map>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <cerrno>
# include <functional>
# include <vector>

# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/epoll.h>

# define ERROR_WRONG_PASSWORD "WRONG PASSWORD"

std::vector<std::string> split_string(std::string str, std::string delimiter);