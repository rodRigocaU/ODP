#ifndef BUFFER_PARSER_HPP_
#define BUFFER_PARSER_HPP_

#include <cctype>
#include <iostream>
#include <queue>
#include <vector>

#include "CommandSettings.hpp"
#include "ConsoleOutput.hpp"

namespace odp{

class BufferParser{
private:
  std::queue<std::size_t> bytes_to_read;
  std::pair<CommandType, std::vector<uint8_t>> settings;
  
public:
  std::pair<CommandType, std::vector<uint8_t>>& getCommnadSettings();
  void setCommandSettings(SenderType senderType, const char& command);
  std::size_t getHeaderSize(SenderType senderType, const char& command);
  std::size_t getContentSize(const std::string& headerString);
  std::vector<std::string> getContentInTokens(const std::string& contentString);
  CommandType getCommandType();
};

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

std::pair<CommandType, std::vector<uint8_t>>& BufferParser::getCommnadSettings(){
  return settings;
}

void BufferParser::setCommandSettings(SenderType senderType, const char& command){
  settings = CommandSettings::getSettings(std::tolower(command), senderType);
}

std::size_t BufferParser::getHeaderSize(SenderType senderType, const char& command){
  setCommandSettings(senderType, command);
  std::size_t sumOfBytes = 0;
  for(std::vector<uint8_t>::iterator::reference nDigit : settings.second){
    sumOfBytes += static_cast<std::size_t>(nDigit);
  }
  return ((settings.first == CommandType::AskList)?settings.second[0]:sumOfBytes);
}

std::size_t BufferParser::getContentSize(const std::string& headerString){
  std::size_t sumOfBytes = 0;
  if(!settings.second.empty()){
    std::size_t beginOfRawData = 0;
    if(settings.first == CommandType::AskList){//SPECIAL CASE
      std::size_t counter = 1;
      for(; counter <= std::stoi(headerString.substr(0, settings.second.front())); ++counter){
        uint32_t value = std::stoi(headerString.substr(counter * 2, settings.second.back()));
        bytes_to_read.push(value);
        sumOfBytes += value;
        clog::ConsoleOutput::print("[BufferParser <BYTE SIZE>]: " + std::to_string(value));
      }
      beginOfRawData = counter * 2;
    }
    else{
      std::size_t counter = 0;
      for(uint8_t& digitCounter : settings.second){
        uint32_t value = std::stoi(headerString.substr(counter, digitCounter));
        bytes_to_read.push(value);
        sumOfBytes += value;
        clog::ConsoleOutput::print("[BufferParser <BYTE SIZE>]: " + std::to_string(value));
        counter += digitCounter;
      }
      beginOfRawData = counter;
    }
  }
  return sumOfBytes;
}

std::vector<std::string> BufferParser::getContentInTokens(const std::string& contentString){
  std::vector<std::string> parsedMessage;
  std::size_t currentPosition = 0;
  while(!bytes_to_read.empty()){
    parsedMessage.push_back(contentString.substr(currentPosition, bytes_to_read.front()));
    clog::ConsoleOutput::print("[BufferParser <EXTRACTED TOKEN>]: " + parsedMessage.back());
    currentPosition += bytes_to_read.front();
    bytes_to_read.pop();
  }
  clog::ConsoleOutput::print("Done");
  return parsedMessage;
}

CommandType BufferParser::getCommandType(){
  return settings.first;
}

}

#endif//BUFFER_PARSER_HPP_