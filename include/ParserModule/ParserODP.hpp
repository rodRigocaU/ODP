#ifndef PARSER_ODP_HPP_
#define PARSER_ODP_HPP_

#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "BufferReader.hpp"
#include "ConsoleOutput.hpp"
#include "TokenFile.hpp"

namespace odp{

//PATTERN->SINGLETON
class ParserODP{
private:
  ParserODP();
  CommandType checkType(const std::string& command);
  std::unordered_map<std::string, std::vector<uint8_t>> commands_user2server;
  std::unordered_map<std::string, std::vector<uint8_t>> commands_server2user;
public:
  static CommandType ProcessBuffer(SenderType sender, const std::string& bufferContent, std::vector<std::string>& container);
  static ParserODP& getGlobalParserODP();
};

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

CommandType ParserODP::ProcessBuffer(SenderType sender, const std::string& bufferContent, std::vector<std::string>& container){
  CommandType command;
  std::stringstream bufferSStream(bufferContent);
  std::string command_str, remainderContent;

  getline(bufferSStream, command_str, TOKEN_SEPARATOR);
  getline(bufferSStream, remainderContent);

  std::unordered_map<std::string, std::vector<uint8_t>>::iterator comm_it;

  if(sender == SenderType::Server)
    comm_it = getGlobalParserODP().commands_server2user.find(command_str);
  else if(sender == SenderType::User)
    comm_it = getGlobalParserODP().commands_user2server.find(command_str);
  clog::ConsoleOutput::print("[ParserODP <COMMAND TOKEN>]: " + command_str);
  command = getGlobalParserODP().checkType(command_str);
  if(!comm_it->second.empty()){
    std::queue<uint32_t> listBytes2Read;
    std::size_t beginOfRawData = 0;
    if(command == CommandType::AskList){//SPECIAL CASE
      std::size_t counter = 1;
      for(; counter <= std::stoi(remainderContent.substr(0, comm_it->second[0])); ++counter){
        uint32_t value = std::stoi(remainderContent.substr(counter * 2, comm_it->second[1]));
        listBytes2Read.push(value);
        clog::ConsoleOutput::print("[ParserODP <BYTE SIZE>]: " + std::to_string(value));
      }
      beginOfRawData = counter * 2;
    }
    else{
      std::size_t counter = 0;
      for(uint8_t& digitCounter : comm_it->second){
        uint32_t value = std::stoi(remainderContent.substr(counter, digitCounter));
        listBytes2Read.push(value);
        clog::ConsoleOutput::print("[ParserODP <BYTE SIZE>]: " + std::to_string(value));
        counter += digitCounter;
      }
      beginOfRawData = counter;
    }
    clog::ConsoleOutput::print("[ParserODP <STATUS>]: Ready to read data.");
    BufferReader::read(listBytes2Read, remainderContent.substr(beginOfRawData), container);
    clog::ConsoleOutput::print("[ParserODP <STATUS>]: All the data read.");
  }
  else
    clog::ConsoleOutput::print("[ParserODP <STATUS>]: No data to read, only the command.");
  return command;
}

CommandType ParserODP::checkType(const std::string& command){
  if(command == TOKEN_COMM_LOGIN)
    return CommandType::Login;
  if(command == TOKEN_COMM_LIST)
    return CommandType::AskList;
  if(command == TOKEN_COMM_MESSUSER)
    return CommandType::UserMessage;
  if(command == TOKEN_COMM_MESSBRC)
    return CommandType::BroadcastMessage;
  if(command == TOKEN_COMM_FILEUSER)
    return CommandType::UploadFile;
  if(command == TOKEN_COMM_FILEBRC)
    return CommandType::BroadcastFile;
  if(command == TOKEN_COMM_FILEACCEPT)
    return CommandType::AcceptFile;
  if(command == TOKEN_COMM_EXIT)
    return CommandType::Exit;
  if(command == TOKEN_COMM_ERROR)
    return CommandType::Error;
  return CommandType::None;
}

ParserODP::ParserODP(){
  commands_user2server[TOKEN_COMM_LOGIN] = {2, 2};
  commands_user2server[TOKEN_COMM_LIST] = {};
  commands_user2server[TOKEN_COMM_MESSUSER] = {2, 2, 3};
  commands_user2server[TOKEN_COMM_MESSBRC] = {2, 3};
  commands_user2server[TOKEN_COMM_FILEUSER] = {2, 2, 1, 2, 5};
  commands_user2server[TOKEN_COMM_FILEBRC] = {2, 1, 2, 5};
  commands_user2server[TOKEN_COMM_FILEACCEPT] = {1};
  commands_user2server[TOKEN_COMM_EXIT] = {2};

  commands_server2user[TOKEN_COMM_ERROR] = {1};
  commands_server2user[TOKEN_COMM_LOGIN] = {};
  commands_server2user[TOKEN_COMM_LIST] = {2, 2};
  commands_server2user[TOKEN_COMM_MESSUSER] = {2, 3};
  commands_server2user[TOKEN_COMM_MESSBRC] = {2, 3};
  commands_server2user[TOKEN_COMM_FILEUSER] = {2, 2, 1, 2, 5};
  commands_server2user[TOKEN_COMM_FILEACCEPT] = {};
  commands_server2user[TOKEN_COMM_EXIT] = {};
}

ParserODP& ParserODP::getGlobalParserODP(){
  static ParserODP INSTANCE_PARSER_ODP;
  return INSTANCE_PARSER_ODP;
}

}

#endif//PARSER_ODP_HPP_