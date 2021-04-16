#ifndef CONSTRUCTOR_MESSAGE_HPP_
#define CONSTRUCTOR_MESSAGE_HPP_

#include <cctype>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include "CommandSettings.hpp"

// using namespace std;

namespace odp
{

  class ConstructorMessage
  {
  private:
    static std::string fitDigits(int n, int digits) 
    {
      std::string zeros(digits - (int)floor(log10(n) + 1), '0');
      return zeros + std::to_string(n);
    }

  public:
    static std::string buildMessage(std::vector<std::string> &data, const char &command, SenderType senderType)
    {
      std::pair<CommandType, std::vector<uint8_t>> settings = CommandSettings::getSettings(std::tolower(command), senderType);
      std::string buildedMessage;
      // Agregar comando
      buildedMessage.push_back(command);

      if(settings.first == CommandType::AskList && senderType == SenderType::Server)
      {
       
        // agregamos la cantidad de usuarios activos
        buildedMessage += fitDigits(data.size(), 2);

        for (auto elem : data)
        {
          buildedMessage += fitDigits(elem.length(), settings.second[1]);
        }
      }
      else
      {
        
        //servidor chiste 
        std::size_t idx = 0;
        for (uint8_t &nDigits : settings.second){
            // std::cout<< buildedMessage <<std::endl; //M -> M004 -> 
            std::cout<< data[idx].length() <<std::endl; 
            buildedMessage += fitDigits(data[idx++].length(), nDigits);
        }
          
          
      }

      // agregamos el contenido del mensaje
      for (const std::string &elem : data)
        buildedMessage += elem;
      
      clog::ConsoleOutput::print("Done");
      

      return buildedMessage;
    }
  };

}

#endif // CONSTRUCTOR_MESSAGE_HPP_