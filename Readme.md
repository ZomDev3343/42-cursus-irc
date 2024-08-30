- Client \
-- 0 > Name Length <= 9 \
-- Operators \

- Channels \
-- Begin with '#' or '&' \
-- Does not contain ' ' or ^G or ','


| **Action**                                    | **HexChat Command**                             | **nc Command**                                                                                                      |
|-----------------------------------------------|------------------------------------------------|---------------------------------------------------------------------------------------------------------------------|
| **Configuration / Connection & Authentication** | - Configure HexChat to connect to `127.0.0.1` on the chosen port. <br> - Use `/PASS mdp` to set the password. <br> - Set the nickname and username when prompted by HexChat. | ```bash <br> nc 127.0.0.1 <port>\n <br> PASS mdp\n <br> NICK <nickname>\n <br> USER <username> 0 * :<realname> ```      |
| **Join a Channel**                            | /JOIN #channel                                  | ```bash <br> JOIN #channel ```                                                                                      |
| **Send a Message to a Channel**               | Just type the message in the chat window.       | ```bash <br> PRIVMSG #channel :This is a message to the channel ```                                                 |
| **Send a Private Message**                    | /PRIVMSG <username> This is a private message   | ```bash <br> PRIVMSG <username> :This is a private message ```                                                      |
| **Kick a User from a Channel**                | /KICK #channel <username> Reason for kicking    | ```bash <br> KICK #channel <username> :Reason for kicking ```                                                       |
| **Invite a User to a Channel**                | /INVITE <username> #channel                     | ```bash <br> INVITE <username> #channel ```                                                                         |
| **Accept an Invite**                          | /JOIN #channel                                  | ```bash <br> JOIN #channel ```                                                                                      |
| **Change/View the Channel Topic**             | /TOPIC #channel New topic for the channel       | ```bash <br> TOPIC #channel :New topic for the channel ```                                                          |
| **Set/Remove Invite-Only Mode**               | /MODE #channel +i or /MODE #channel -i          | ```bash <br> MODE #channel +i <br> MODE #channel -i ```                                                             |
| **Set/Remove Topic Restriction Mode**         | /MODE #channel +t or /MODE #channel -t          | ```bash <br> MODE #channel +t <br> MODE #channel -t ```                                                             |
| **Set/Remove Channel Key (Password)**         | /MODE #channel +k <password> or /MODE #channel -k | ```bash <br> MODE #channel +k <password> <br> MODE #channel -k ```                                                 |
| **Give/Remove Operator Privilege**            | /MODE #channel +o <username> or /MODE #channel -o <username> | ```bash <br> MODE #channel +o <username> <br> MODE #channel -o <username> ```                                      |
| **Set/Remove User Limit**                     | /MODE #channel +l <limit> or /MODE #channel -l  | ```bash <br> MODE #channel +l <limit> <br> MODE #channel -l ```                                                    |
