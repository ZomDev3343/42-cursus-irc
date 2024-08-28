#pragma once

/* Error Messages */

# define ERR_PASSWORD(source) "464 " + source + ":Password incorrect\r\n"
# define ERR_NOTREGISTERED(source) "451 " + source + " :You have not registred\r\n"
# define ERR_ALREADY_REGISTERED(source) "462 " + source + " :You may not reregister\r\n"
# define ERR_NOTONCHANNEL(source, channel) "442 " + source + " " + channel + " :You're not on that channel\r\n"
# define ERR_USERNOTINCHANNEL(source, nickname, channel) "441 " + source + " " + nickname + " " + channel + " :They aren't on that channel\r\n"
# define ERR_NOSUCHCHANNEL(source, channel) "403 " + source + " " + channel + " :No such channel\r\n"
# define ERR_CHANOPRIVSNEEDED(source, channel) "482 " + source + " " + channel + " :You're not channel operator\r\n"
# define ERR_NOSUCHNICK(source, name) "401 " + source + " " + name + " :No such nick/channel\r\n"
# define ERR_KICKOPERATOR(source) "666 " + source + " :Can't kick an operator\r\n"


/* Commands Messages */

# define KICK_RPL(kicker, kicking, channel, reason) ":" + kicker + " KICK " + channel + " " + kicking + " :" + reason + "\r\n"
# define WHO_RPL(channel, username, hostname, server, nickname) "352 " + channel + " " + username + " " + hostname + " " + server + " " + nickname + " H :0 " + nickname + "\r\n"
# define ENDOFWHO_RPL(nickname) "315 " + nickname + " :End of /WHO list\r\n"
