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
  void readCommLogin();
  std::unordered_map<std::string, std::vector<uint8_t>> commands_user2server, commands_server2user;
public:
  static CommandType ProcessBuffer(SenderType sender, const std::string& bufferContent);
};

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

CommandType ParserODP::ProcessBuffer(SenderType sender, const std::string& bufferContent){
  static ParserODP INSTANCE_PARSER_ODP;
}

ParserODP::ParserODP(){
  commands_user2server[TOKEN_COMM_LOGIN] = {2,2};
}

}

#endif//PARSER_ODP_HPP_