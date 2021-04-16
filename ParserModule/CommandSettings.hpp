#ifndef COMMAND_SETTINGS_HPP_
#define COMMAND_SETTINGS_HPP_

#include <vector>
#include <string>
#include <unordered_map>

#include "TokenFile.hpp"

namespace odp{
//PATTERN->SINGLETON
class CommandSettings{
private:
  typedef std::pair<CommandType, std::vector<uint8_t>> Settings;
  std::unordered_map<char, Settings> user2server;
  std::unordered_map<char, Settings> server2user;


  CommandSettings();
public:
  static CommandSettings& getGlobalCommandSettings();
  static std::pair<CommandType, std::vector<uint8_t>>& getSettings(const char& command, SenderType senderType);
};

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

CommandSettings::CommandSettings(){
  user2server[TOKEN_COMM_LOGIN] = {CommandType::Login, {2, 2}};
  user2server[TOKEN_COMM_LIST] = {CommandType::AskList, {}};
  user2server[TOKEN_COMM_MESSAGE_USER] = {CommandType::UserMessage, {3, 2}};
  user2server[TOKEN_COMM_MESSAGE_BROADCAST] = {CommandType::BroadcastMessage, {3}};
  user2server[TOKEN_COMM_UPLOAD_FILE] = {CommandType::UploadFile, {3, 10, 2}};
  user2server[TOKEN_COMM_ACCEPT_FILE] = {CommandType::AcceptFile, {2}};
  user2server[TOKEN_COMM_EXIT] = {CommandType::Exit, {}};

  server2user[TOKEN_COMM_LOGIN] = {CommandType::Login, {}};
  server2user[TOKEN_COMM_LIST] = {CommandType::AskList, {2, 2}};
  server2user[TOKEN_COMM_MESSAGE_USER] = {CommandType::UserMessage, {3, 2}};
  server2user[TOKEN_COMM_MESSAGE_BROADCAST] = {CommandType::BroadcastMessage, {3, 2}};
  server2user[TOKEN_COMM_UPLOAD_FILE] = {CommandType::UploadFile, {3, 10, 2}};
  server2user[TOKEN_COMM_ACCEPT_FILE] = {CommandType::AcceptFile, {2}};
  server2user[TOKEN_COMM_EXIT] = {CommandType::Exit, {}};
  server2user[TOKEN_COMM_ERROR] = {CommandType::Error, {2}};
}

CommandSettings& CommandSettings::getGlobalCommandSettings(){
  static CommandSettings INSTANCE_COMMAND_SETTINGS;
  return INSTANCE_COMMAND_SETTINGS;
}

std::pair<CommandType, std::vector<uint8_t>>& CommandSettings::getSettings(const char& command, SenderType senderType){
  std::unordered_map<char, Settings>::iterator it;
  if(senderType == SenderType::User){
    it = getGlobalCommandSettings().user2server.find(command);
    if(it != getGlobalCommandSettings().user2server.end())
      return it->second;
  }
  else if(senderType == SenderType::Server){
    it = getGlobalCommandSettings().server2user.find(command);
    if(it != getGlobalCommandSettings().server2user.end())
      return it->second;
  }
  return it->second;
}

}

#endif//COMMAND_SETTINGS_HPP_