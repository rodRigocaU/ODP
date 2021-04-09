#include "ParserModule.hpp"

int main(){
  std::cout << "im server\n";
  //EXAMPLE:
  //clog::ConsoleOutput::enableOutput(0); //set 0 if you want omit the outputs of the parser
  std::vector<std::string> rawData;
  odp::CommandType comm_type = odp::ParserODP::ProcessBuffer(odp::SenderType::User, "fu#050330400015juliowootxtholahola como estas", rawData);
  for(auto& i : rawData){
    std::cout << i << std::endl;
  }
  return 0;
}