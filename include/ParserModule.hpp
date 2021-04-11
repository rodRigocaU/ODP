#ifndef PARSER_MODULE_HPP_
#define PARSER_MODULE_HPP_

/*This file has to be included in client.cpp and server.cpp
Calls:
  std::vector<std::string> rawData;//sender could be odp::SenderType::User or odp::SenderType::Server
  odp::CommandType commType = odp::ParserODP(sender, buffer, rawData);//buffer is the data given from the socket
*/

#include "ParserModule/ConsoleOutput.hpp"
#include "ParserModule/BufferParser.hpp"

#endif//PARSER_MODULE_HPP_