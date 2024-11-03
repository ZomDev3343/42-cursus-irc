# IRC Server in C++98 with epoll and fcntl

## Table of Contents

- [Introduction](#introduction)
- [What is IRC?](#what-is-irc)
  - [Brief History](#brief-history)
  - [What is an IRC Server?](#what-is-an-irc-server)
- [The IRC Protocol](#the-irc-protocol)
  - [Overview](#overview)
  - [Syntax](#syntax)
- [HexChat](#hexchat)
- [Client and Channel Naming Conventions](#client-and-channel-naming-conventions)
  - [Client Names](#client-names)
  - [Channel Names](#channel-names)
- [Operators](#operators)
  - [Definition](#definition)
  - [Commands](#commands)
- [epoll and fcntl](#epoll-and-fcntl)
  - [Understanding epoll and fcntl](#understanding-epoll-and-fcntl)
  - [How They Work Together](#how-they-work-together)
  - [Why epoll over poll?](#why-epoll-over-poll)
- [Bot Functionality](#bot-functionality)
- [File Download with CTCP and DCC](#file-download-with-ctcp-and-dcc)
  - [CTCP and DCC Protocols](#ctcp-and-dcc-protocols)
  - [Commands](#commands)
- [Usage](#usage)
  - [Commands Reference](#commands-reference)
- [Error Handling](#error-handling)
- [Conclusion](#conclusion)
- [Contributors](#contributors)
- [Documentation](#footnotes)


---

## Introduction

This project is an implementation of an IRC (Internet Relay Chat) server in C++98, utilizing epoll and fcntl for efficient I/O multiplexing and non-blocking I/O operations. The server is designed to work seamlessly with HexChat, a popular IRC client, and supports essential IRC functionalities along with additional features like bot interaction and file transfers using DCC (Direct Client-to-Client).

---

## What is IRC?

### Brief History

Internet Relay Chat (IRC) is a real-time text messaging protocol that facilitates communication in the form of chat rooms, private messages, and file sharing. Developed by Jarkko Oikarinen in 1988, IRC was one of the first protocols to enable multi-user chat over the internet. It has played a significant role in online communication, influencing many modern messaging platforms.

### What is an IRC Server?

An IRC server is a software application that implements the IRC protocol, allowing clients to connect, join channels (chat rooms), and communicate with other users. The server handles user authentication, message routing, channel management, and maintains the network of connected clients.

---

## The IRC Protocol

### Overview

The IRC protocol is a text-based protocol that uses TCP (Transmission Control Protocol) for reliable communication. It defines a set of commands and replies for client-server and server-server communication. Users can send messages to channels or other users, change nicknames, join or leave channels, and perform various administrative functions.

### Syntax

IRC messages follow a specific format:

```
[":" prefix " "] command [" " params] [":" trailing]
```

- **Prefix**: Optional; indicates the sender of the message.
- **Command**: A keyword or numeric indicating the action (e.g., JOIN, PRIVMSG).
- **Params**: Parameters required by the command.
- **Trailing**: The last parameter, prefixed with : can contain spaces.

**Example:**

```
PRIVMSG #channel :Hello, world!
```

This command sends a message to #channel with the content "Hello, world!".

---

## HexChat

HexChat is an easy-to-use, customizable, and feature-rich IRC client available on multiple platforms. It allows users to connect to IRC servers, join channels, and communicate with others. HexChat provides a graphical user interface, making it accessible for users unfamiliar with command-line clients.

---

## Client and Channel Naming Conventions

### Client Names

- **Length**: Must be between 1 and 9 characters.
- **Restrictions**:
  - Should avoid invalid characters.
  - Must comply with IRC specifications.
  - Typically, alphanumeric characters and certain symbols are allowed.

### Channel Names

- **Starting Character**: Must begin with # or &.
- **Restrictions**:
  - Cannot contain spaces ( ).
  - Cannot contain control-G (^G).
  - Cannot contain commas (,).

---

## Operators

### Definition

Operators are users with elevated privileges within a channel. They have the authority to manage other users, modify channel settings, and enforce channel rules. Operators can kick users, set channel modes, and grant or revoke operator status to other users.

### Commands

Operators can manage privileges using mode commands:

- **Grant Operator Status**:

```
MODE #channel +o username
```

- **Revoke Operator Status**:

```
MODE #channel -o username
```

**Example Usage in HexChat:**

To give operator privileges to a user named Alice in #chatroom:

```
/MODE #chatroom +o Alice
```

---

## epoll and fcntl

### Understanding epoll and fcntl

#### epoll

- **Purpose**: epoll is an I/O event notification facility specific to Linux, designed to efficiently handle large numbers of file descriptors.
- **Functionality**:
  - Monitors multiple file descriptors to see if I/O is possible on any of them.
  - Uses an event-driven approach, notifying the application when events occur.
- **Advantages**:
  - Scalable and efficient for servers handling many simultaneous connections.
  - Outperforms traditional methods like poll or select in high-load scenarios.

#### fcntl

- **Purpose**: fcntl (file control) performs various operations on file descriptors, including changing properties and behaviors.
- **Functionality**:
  - Sets file descriptor flags, such as non-blocking mode (O_NONBLOCK).
  - Controls file locks, duplication, and other advanced features.

### How They Work Together

In a non-blocking server, fcntl is used to set sockets to non-blocking mode, allowing I/O operations to return immediately without waiting. This is crucial when using epoll, as it ensures the server can handle other tasks or connections while waiting for I/O operations to complete.

By combining epoll and fcntl, the server can efficiently monitor multiple connections and perform asynchronous I/O operations, resulting in better performance and scalability.

### Why epoll over poll?

- **Performance**:
  - epoll is more efficient with a large number of file descriptors because it doesn't need to iterate over all of them to detect events.
  - poll becomes less efficient as the number of monitored descriptors increases.
- **Scalability**:
  - epoll scales better for high-load servers with many simultaneous connections.
- **Edge-Triggered Notifications**:
  - epoll supports both level-triggered and edge-triggered notifications, providing more control over event handling.

---

## Bot Functionality

The server includes a simple bot that automatically joins channels and interacts with users. When a user joins a channel, the bot sends a greeting message.

---

## File Download with CTCP and DCC

### CTCP and DCC Protocols

#### CTCP (Client-To-Client Protocol)

- **Purpose**: Extends the IRC protocol to allow clients to send direct messages and commands to each other.
- **Functionality**:
  - Encapsulates messages within PRIVMSG or NOTICE commands using special delimiters (\x01).
  - Used for actions like requesting client versions, pinging, or initiating file transfers.

#### DCC (Direct Client-to-Client)

- **Purpose**: Enables direct connections between clients for file transfers or private chats, bypassing the server after the initial handshake.
- **Functionality**:
  - Uses CTCP to initiate a direct connection.
  - Clients establish a TCP connection directly for data exchange.

### Commands

To initiate a file transfer using DCC:

```
PRIVMSG Bob :\x01DCC SEND filename ip port filesize\x01
```

- **Bob**: The recipient's nickname.
- **filename**: The name of the file being sent.
- **ip**: The sender's IP address in a 32-bit unsigned integer format.
- **port**: The port number the sender is listening on.
- **filesize**: The size of the file in bytes.

**In HexChat**:

- **Sending a File**:
  - Right-click on the user's name and select "Send File".
  - Choose the file to send.
- **Receiving a File**:
  - Accept the file transfer request.
  - Choose where to save the file.

---

## Usage

### Commands Reference

| **Action**                                    | **HexChat Command**                             | **Netcat Command**                                                                                                  |
|-----------------------------------------------|-------------------------------------------------|---------------------------------------------------------------------------------------------------------------------|
| **Configuration / Connection & Authentication** | - Configure HexChat to connect to 127.0.0.1 on the chosen port.<br>- Use /PASS \<password\> to set the password.<br>- Set the nickname and username when prompted by HexChat. | <br>nc 127.0.0.1 \<port\><br>PASS \<password\><br>NICK \<nickname\><br>USER \<username\> 0 * :\<realname\> |
| **Join a Channel**                            | /JOIN #channel                                  | <br>JOIN #channel                                                                                        |
| **Send a Message to a Channel**               | Just type the message in the chat window.       | <br>PRIVMSG #channel :This is a message to the channel                                                   |
| **Send a Private Message**                    | /PRIVMSG \<username\> This is a private message | <br>PRIVMSG \<username\> :This is a private message                                                      |
| **Kick a User from a Channel**                | /KICK #channel \<username\> Reason for kicking  | <br>KICK #channel \<username\> :Reason for kicking                                                       |
| **Invite a User to a Channel**                | /INVITE \<username\> #channel                   | <br>INVITE \<username\> #channel                                                                         |
| **Accept an Invite**                          | /JOIN #channel                                  | <br>JOIN #channel                                                                                        |
| **Change/View the Channel Topic**             | /TOPIC #channel New topic for the channel       | <br>TOPIC #channel :New topic for the channel                                                            |
| **Set/Remove Invite-Only Mode**               | /MODE #channel +i or /MODE #channel -i          | <br>MODE #channel +i<br>MODE #channel -i                                                                |
| **Set/Remove Topic Restriction Mode**         | /MODE #channel +t or /MODE #channel -t          | <br>MODE #channel +t<br>MODE #channel -t                                                                 |
| **Set/Remove Channel Key (Password)**         | /MODE #channel +k \<password\> or /MODE #channel -k | <br>MODE #channel +k \<password\><br>MODE #channel -k                                             |
| **Give/Remove Operator Privilege**            | /MODE #channel +o \<username\> or /MODE #channel -o \<username\> | <br>MODE #channel +o \<username\><br>MODE #channel -o \<username\>                                      |
| **Set/Remove User Limit**                     | /MODE #channel +l \<limit\> or /MODE #channel -l | <br>MODE #channel +l \<limit\><br>MODE #channel -l                                                     |

---

## Error Handling

The IRC server implements comprehensive error handling to provide clear feedback to users when issues occur. Below is a summary of the managed errors:

- **ERR_PASSWORD (464)**: Password incorrect. Triggered when a user provides a wrong password upon connecting.

- **ERR_NOTREGISTERED (451)**: You have not registered. Triggered when attempting commands without setting a nickname and username.

- **ERR_ALREADY_REGISTERED (462)**: You may not reregister. Triggered when trying to register again after initial registration.

- **ERR_NOTONCHANNEL (442)**: You're not on that channel. Triggered when performing actions on a channel without joining it.

- **ERR_USERNOTINCHANNEL (441)**: They aren't on that channel. Triggered when addressing a user who isn't present in the channel.

- **ERR_NOSUCHCHANNEL (403)**: No such channel. Triggered when referencing a non-existent channel.

- **ERR_CHANOPRIVSNEEDED (482)**: You're not channel operator. Triggered when non-operators attempt operator-only actions.

- **ERR_NOSUCHNICK (401)**: No such nick/channel. Triggered when interacting with an unconnected nickname.

- **ERR_KICKOPERATOR (666)**: Can't kick an operator. Triggered when attempting to kick an operator without rights.

- **ERR_KEYSET (467)**: Channel key already set. Triggered when setting a password on an already protected channel.

- **ERR_BADCHANNELKEY (475)**: Cannot join channel (+k). Triggered when joining a password-protected channel with the wrong key.

- **ERR_TOOMANYCHANNELS (405)**: You have joined too many channels. Triggered when exceeding the maximum allowed channels.

- **ERR_INVITEONLYCHAN (473)**: Cannot join channel (+i). Triggered when joining an invite-only channel without an invite.

- **ERR_CHANNELISFULL (471)**: Cannot join channel (+l). Triggered when attempting to join a full channel.

- **ERR_NOLOGIN (444)**: User not logged in. Triggered when actions require login, but the user isn't logged in.

- **ERR_USERONCHANNEL (443)**: Is already on channel. Triggered when inviting a user who is already a member.

- **ERR_NONICKNAMEGIVEN (431)**: No nickname given. Triggered when commands require a nickname, but none is given.

- **ERR_NICKNAMEINUSE (433)**: Nickname is already in use. Triggered when setting a nickname that is taken by another user.

These errors adhere to IRC specifications and assist users in correcting issues promptly.

---

## Conclusion

This IRC server project demonstrates a functional implementation of the IRC protocol in C++98, featuring efficient networking using epoll and fcntl, integration with HexChat, and support for essential IRC commands and features. The inclusion of a bot, comprehensive error handling, and support for file transfers via DCC adds practical utility and showcases advanced aspects of IRC server capabilities.

---

## Contributors

Feel free to contribute, report issues, or suggest enhancements via GitHub.

**Our 42 intra usernames**: truello, thoribal, jedurand

**Our GitHub profiles**:

- [ZomDev3343](https://github.com/ZomDev3343)
- [ThomasRibalta](https://github.com/ThomasRibalta)
- [Beorlor](https://github.com/Beorlor)

---

# Footnotes

- **HexChat**: [https://hexchat.github.io/](https://hexchat.github.io/)
- **IRC Protocol Specification**: [RFC 1459](https://tools.ietf.org/html/rfc1459)
- **epoll Manual**: [epoll(7) - Linux manual page](http://man7.org/linux/man-pages/man7/epoll.7.html)
- **fcntl Manual**: [fcntl(2) - Linux manual page](http://man7.org/linux/man-pages/man2/fcntl.2.html)

