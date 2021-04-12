#include "ParserModule.hpp"
#include "Netconf.hpp"
#include "ServerHandler.hpp"
#include "Utils.hpp"
#include <thread>
#include <utility>
#include <string>

using namespace std;

int main()
{
  odp::ServerHandler s_handler; // server_handler

  struct sockaddr_storage their_addr; // se utilizará en la función accept()
  socklen_t addr_size;                // se utilizará en la función accept()

  struct addrinfo *res;

  setaddressinfo("50000", res);

  int sockfd;
  setmainsock(res, sockfd);

  addr_size = sizeof their_addr;
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

    std::string newusername = s_handler.registeruser(connectfd); 
    if (!newusername.empty()) // si el nombre de usuario no es vacío
    {
      // soltamos un nuevo thread
      thread(&odp::ServerHandler::handleuser, &s_handler, newusername).detach();
    }
  }
  close(sockfd);
  return 0;
}
