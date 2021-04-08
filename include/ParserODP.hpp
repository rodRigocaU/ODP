#ifndef PARSER_ODP_HPP_
#define PARSER_ODP_HPP_

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "TokenFile.hpp"

namespace odp{

enum SenderType{
  User, Server
};

enum CommandType{
  Login,
  AskList,
  UserMessage,
  BroadcastMessage,
  UploadFile,
  BroadcastFile,
  AcceptFile,
  Exit,
  Error
};

//PATTERN->SINGLETON
class ParserODP{
private:
  ParserODP();
  
  std::unordered_map<std::string, std::vector<uint8_t>> commands_settings;
public:
  static CommandType ProcessBuffer(const std::string& bufferContent);
};

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

CommandType ParserODP::ProcessBuffer(const std::string& bufferContent){
  static ParserODP INSTANCE_PARSER_ODP;
}

ParserODP::ParserODP(){
  commands_settings[TOKEN_COMM_LOGIN_U2S] = {1,2,3};
}

}

#endif//PARSER_ODP_HPP_