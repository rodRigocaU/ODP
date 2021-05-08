/* Client code in C++ */
// g++ -std=c++11 -o server.exe server.cpp
#include "Handlers/ClientHandler.hpp"
#include "Handlers/Netconf.hpp"
#include "Tools/HiddenPassword.hpp"

#include <thread>
#include <iostream>
#include <string>

using namespace std;

int main()
{
  clog::ConsoleOutput::enableOutput(0);
  struct addrinfo *res;

  setaddressinfo(CIPSERVER, CPORT, res, false);

  // ahora creamos el socket de conexión
  int sockfd;

  setmainsock(res, sockfd, 10, false);

  // hasta aquí hemos usado la variable res, así que liberamos la memoria de esta lista

  char answer[3];

  std::vector<std::string> send_data(2); // send_data = [username, password]

  while (strcmp(answer,"Lok"))
  {
    // agrego esto porque fallaba al intentar reinsertar otro nombre de usuario, porque el primero fué rechazado
    send_data[0].clear(); 
    send_data[1].clear();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::cout << "Ingrese Nombre de usuario y password:" << std::endl;

    std::cout << "Username:";
    std::getline(cin, send_data[0]);
    std::cout << "Password:";
    odp::getPassword(send_data[1]);
    
    std::string message = odp::ConstructorMessage::buildMessage(send_data, 'l', odp::SenderType::User);

    int n = write(sockfd, message.c_str(), message.length());

    char buffer[30];
    n = read(sockfd, buffer, 30);
    buffer[n]='\0';
    strcpy(answer,buffer);

    //---------------------
    std::cout<<"Size: "<< n <<"\n";
    std::cout<<"Respuesta del Server: "<<answer<<"\n";
    //------------------------
  }

  cout << GRN2 "///****,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,........,,,,,,,,,,,,,,,,**************" NC<< '\n';
	cout << GRN2 "//***,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,...,,,,,......,,,,.,,,,,,,,,,,,*,**********\n"  NC;
	cout << GRN2 "****,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,....*/(########(#*.,,,,,,,,*******\n" NC;
	cout << GRN2 ",,,,,,,,,,,....,,....%(%,,,,,,,,.,,.,,,,,.*(/(##%#(%%%##%%####&&%%%%(,,,,,,,,,**\n" NC;
	cout << GRN2 ",,,,,,,,,,....*//((%#&%%######%##%#(((((##%%#######%%&%&%%%%%%&&&%%&%%%#,,,,,,,,\n" NC;
	cout << GRN2 ",,,,,,,,..*/((/((*(/((###((##(#&#((##((((/#%%%%%#(#######%#%%#%%%&%&&%%%&%,,,,,,\n" NC;
	cout << GRN2 ",,,,,,.,*(##((((((((####((#%(/#%%#((#(((#%######(##########%%%%%%%&%&&%&&&&&,,,,\n" NC;
	cout << GRN2 ",,,,,,.(&%####(####%%%%&@&%%((#%#%%&&&%%#%#########(####%#%%%%&%&&&&@&&&&&@&@&,,\n" NC;
	cout << GRN2 ",,,,,,,./##(%%&%%&%&&&%&&&%##%##%%%#%%%####%##%#%####%%%%%%%%%%&&&@&&&&&&%%@&@#/\n" NC;
	cout << GRN2 ",,,,,,,,..*%%#/,,,,,**%%%&%%%#%%%##%%##%%%%%%%#%#%%%%%%%&&&&&%%&&&&&&@&&&&@@@%**\n" NC;
	cout << GRN2 ",,,,,,,,,,,,,,,,,,,,,,,*#&&%%&%%%#%%#%%%%%&&%&&%%%&%%&&%&&%%%%%&&&&@@@&&&&&@%/*,\n" NC;
	cout << GRN2 ",,,,,,,,,,,,,,,....,,,.,,,#&%&&&%%&&&&&%#%%&&&%&&&&&&&&&&&%%%&%&@@@&@&%@@@%/*,,,\n" NC;
	cout << GRN2 ",,,,,,,,,,,,,,,,..,.........(%%%##%&&@@@@&&&&&&&&&&&&&@@@&&%%%%&&&&@&@@@@%/*,,,,\n" NC;
	cout << GRN2 ",,,,,,,,,,,,,,,,.............,(##/##%&%&&&%&@%&&@&&@&@@@@@@&%&%&&@&@@@#@@@/*,***\n" NC;
	cout << GRN2 ",,,,,,,,,,,,,,,,.............../((%#%&&&&@@&%#,...........,,,(&&@@@@@@@@@@@/****\n" NC;
	cout << GRN2 ",,*,,,,,,,,,,........,,,,,....*(%#%%*.*&&&&*.........,,,,,,,,,,&@&@@@/@@@@&*****\n" NC;
	cout << GRN2 ",****,,,,,,........,,,.....,,,,%&&(,.,,.%&&/..,,,,,,,..,,,,,,,,%&@&***&@@#******\n" NC;
	cout << GRN2 "**,,,,,,,,,,,,,,,,,,,..,,,,,..*#@@,,,,,,#&%#.,.,,,,,,,,,,,,,,#%@@/**/&@@********\n" NC;
	cout << GRN2 "***,*,,*,,,,,,,,,*,,,,,,,,,,(#&@@(,,,,#%&&@#,,,,,,,,,***&@%&@@&#(/////////////*/\n" NC;
	cout << GRN2 "**,**************,,,,,*****,,,,,,,,*,,,,,,,,,,,,,,,,****************************\n" NC;
	cout << GRN2 "***************************,**,,,,,,,*,,,,,,,,,,,,*,,,****,**************/****//\n" NC;



  odp::ClientHandler c_handler(sockfd, send_data[0], send_data[1]);
  // escuchamos al servidor
  std::thread(&odp::ClientHandler::handlerecv, &c_handler).detach();

  // Enviar Comandos al servidor
  c_handler.handlesend();

  return 0;
}
