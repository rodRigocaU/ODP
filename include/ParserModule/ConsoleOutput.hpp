#ifndef CONSOLE_OUTPUT_HPP_
#define CONSOLE_OUTPUT_HPP_

#include <iostream>
#include <string>

namespace clog{

//PATTERN->SINGLETON
class ConsoleOutput{
private:
  bool active;
  ConsoleOutput();
public:
  static ConsoleOutput& getGlobalConsoleOutput();
  static void print(const std::string& message);
  static void enableOutput(bool enabled);
};

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

ConsoleOutput::ConsoleOutput(){
  active = true;
}

ConsoleOutput& ConsoleOutput::getGlobalConsoleOutput(){
  static ConsoleOutput INSTANCE_CONSOLE_OUTPUT;
  return INSTANCE_CONSOLE_OUTPUT;
}

void ConsoleOutput::enableOutput(bool enabled){
  getGlobalConsoleOutput().active = enabled;
}

void ConsoleOutput::print(const std::string& message){
  if(getGlobalConsoleOutput().active)
    std::cout << message << std::endl;
}

}

#endif//CONSOLE_OUTPUT_HPP_