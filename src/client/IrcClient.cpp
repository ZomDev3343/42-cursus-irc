#include <iostream>
#include <cstring>
#include <cerrno>
#include "../../include/Irc.hpp"
#include "../../include/IrcClient.hpp"

// Constructeur
IrcClient::IrcClient(int id, std::string host) 
    : _id(id), _hostname(host), _rankId(0), _channel(NULL) {
    // Initialisation des autres membres si nécessaire
}

// Destructeur
IrcClient::~IrcClient() {
    // Libération des ressources si nécessaire
}

// Récupérer l'ID
int const& IrcClient::getId() const {
    return this->_id;
}

// Récupérer le nom d'hôte
std::string const& IrcClient::getHostname() const {
    return this->_hostname;
}

// Récupérer le pseudo
std::string const& IrcClient::getNickname() const {
    return this->_nickname;
}

// Définir le pseudo
void IrcClient::setNickname(std::string newNickname) {
    if (this->_nickname.empty()) {
        this->_nickname = newNickname;
    }
}

// Récupérer le dernier message
std::string const& IrcClient::getLastMessage() const {
    return this->_lastmsg;
}

// Ajouter une partie de message
bool IrcClient::appendMessagePart(std::string& msg_part) {
    std::size_t endcharpos;
    this->_lastmsg.append(msg_part);
    endcharpos = this->_lastmsg.find('\r');
    if ((endcharpos != this->_lastmsg.npos && this->_lastmsg[endcharpos + 1] == '\n') ||
        this->_lastmsg.find('\n') != this->_lastmsg.npos) {
        return true;
    }
    return false;
}

// Effacer le dernier message
void IrcClient::clearLastMessage() {
    this->_lastmsg.clear();
}

// Envoyer un message au client
void IrcClient::sendMessage(std::string message) {
    std::string formattedMessage = message + "\r\n";
    std::cout << "Sending message to client " << this->_id << " : " << message << std::endl;
    if (send(this->_id, formattedMessage.c_str(), formattedMessage.size(), 0) == -1) {
        std::cerr << "Error while sending message to client " << this->_id 
                  << " : " << strerror(errno) << std::endl;
    }
}

// Définir le nom d'hôte
void IrcClient::setHostname(std::string newHost) {
    this->_hostname = newHost;
}

// Définir le dernier message
void IrcClient::setLastMessage(std::string newLastMessage) {
    this->_lastmsg = newLastMessage;
}

// Définir l'ID de rang
void IrcClient::setRankId(int newRankId) {
    this->_rankId = newRankId;
}

// Récupérer l'ID de rang
int IrcClient::getRankId() {
    return this->_rankId;
}

// Définir le canal
void IrcClient::setChannel(Channel* channel) {
    this->_channel = channel;
}

// Récupérer le canal
Channel* IrcClient::getChannel() {
    return this->_channel;
}

// Définir le nom d'utilisateur
void IrcClient::setUsername(std::string username) {
    this->_username = username;
}

// Récupérer le nom d'utilisateur
std::string IrcClient::getUsername() const {
    return this->_username;
}

// Définir le nom réel
void IrcClient::setRealname(std::string realname) {
    this->_realname = realname;
}

// Récupérer le nom réel
std::string IrcClient::getRealname() const {
    return this->_realname;
}

