#include "ParserModule.hpp"
#include "Netconf.hpp"
#include "ServerHandler.hpp"

#include <thread>
#include <iostream>

int main()
{

  struct sockaddr_storage their_addr; // se utilizará en la función accept()
  socklen_t addr_size;                // se utilizará en la función accept()

  struct addrinfo *res;

  setaddressinfo(NULL, "50000", res, true);

  int sockfd;
  setmainsock(res, sockfd);

  addr_size = sizeof their_addr;

  odp::ServerHandler s_handler; // server_handler
  while (true)
  {
    // el nuevo file descriptor resultante de la conexión
    int connectfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (0 > connectfd)
    {
      perror("error accept failed");
      close(sockfd);
      exit(EXIT_FAILURE);
    }

    cout << "Nuevo cliente conectado!" << endl;
    // imprimimos la dirección del nuevo cliente aceptado
    get_in_information(&their_addr);

    // soltamos un nuevo thread
    std::thread(&odp::ServerHandler::handleuser, &s_handler, connectfd).detach();
  }
  return 0;
}
