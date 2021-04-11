#ifndef BUFFER_READER_HPP_
#define BUFFER_READER_HPP_

#include <queue>
#include <string>
#include <vector>

#include "TokenFile.hpp"
#include "ConsoleOutput.hpp"

namespace odp{
  //TYPE->NO INSTANTIABLE
  class BufferReader{
  public:
    static void read(std::queue<uint32_t>& listBytes2Read, const std::string& content, std::vector<std::string>& container);
  };

  //|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  //|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
  //|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

  void BufferReader::read(std::queue<uint32_t>& listBytes2Read, const std::string& content, std::vector<std::string>& container){
    std::size_t currentPosition = 0;
    while(!listBytes2Read.empty()){
      container.push_back(content.substr(currentPosition, listBytes2Read.front()));
      clog::ConsoleOutput::print("[ParserODP::BufferReader <EXTRACTED TOKEN>]: " + container.back());
      currentPosition += listBytes2Read.front();
      listBytes2Read.pop();
    }
  }

}

#endif//BUFFER_READER_HPP_
