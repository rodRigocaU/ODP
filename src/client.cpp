/* Client code in C++ */
#include "ParserModule.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <thread>
#include <iostream>
#include <string>

using namespace std;

#define IPSERVER "127.0.0.1" // el IP del servidor, por ahora localhost
#define PORT "50000"              // el puerto en el que escucha el servidor


int main()
{
  int status;
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     // use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

  // validando la función getaddrinfo
  // llenaremos toda la información relevante en nuestra estructura res, con los datos
  // de nuestro servidor local, p.ej la IP local "127.0.0.1" y el puerto 45000 en este caso
  if ((status = getaddrinfo(IPSERVER, PORT, &hints, &res)) != 0)
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }
  // ahora creamos el socket de conexión
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if (-1 == sockfd)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {
      perror("connect failed");
      close(sockfd);
      exit(EXIT_FAILURE);
    }

  // hasta aquí hemos usado la variable res, así que liberamos la memoria de esta lista
  freeaddrinfo(res);

  int nbytes;
  string msg = "lo#1104santistebanucsp";
  nbytes = send(sockfd, msg.c_str(), msg.size(), 0);
  
  shutdown(sockfd, SHUT_RDWR);
  close(sockfd);
  return 0;
}
