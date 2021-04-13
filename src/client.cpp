/* Client code in C++ */
#include "ParserModule.hpp"
#include "Netconf.hpp"
#include "ClientHandler.hpp"

#include <thread>
#include <iostream>
#include <thread>
#include <string>

using namespace std;

#define IPSERVER "127.0.0.1" // el IP del servidor, por ahora localhost
#define PORT "50000"         // el puerto en el que escucha el servidor

int main()
{
  struct addrinfo *res;

  setaddressinfo(IPSERVER, PORT, res, false);

  // ahora creamos el socket de conexión
  int sockfd;

  setmainsock(res, sockfd, 10, false);

  // hasta aquí hemos usado la variable res, así que liberamos la memoria de esta lista

  std::string answer = "";

  std::string username, password;

  while (answer != "Lok")
  {
    answer.clear();
    std::cout << "Ingrese Nombre de usuario y password:" << std::endl;

    std::cout << "Username:";
    std::getline(cin, username);
    std::cout << "Password:";
    std::getline(cin, password);

    std::string message = odp::ConstructorMessage::buildMessage(std::vector<std::string>({username, password}), 'l', odp::SenderType::Server);

    write(sockfd, message.c_str(), message.length());

    char buffer[3];
    read(sockfd, buffer, 3);
    answer = buffer;
  }

  odp::ClientHandler c_handler(sockfd, username, password);
  
  // Escuchar al servidor
  std::thread(&odp::ClientHandller::handlerecv, &c_handler).detach();
  
  // Enviar Comandos al servidor
  c_handler.handlesend();
  
  
  return 0;
}
