#ifndef NETSET_HPP_
#define NETSET_HPP_
// Networks
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// C++
#include <iostream>


void get_in_information(struct sockaddr_storage *their_addr)
{
  if (their_addr->ss_family == AF_INET)
    {
      // significa que mi cliente usa IPv4
      // casteamos their_addr a sockaddr_in (para IPv4)
      struct sockaddr_in *temp_sockaddr_in = (struct sockaddr_in *)their_addr;
      char ip4[INET_ADDRSTRLEN];                                               // space to hold the IPv4 string
      inet_ntop(AF_INET, &(temp_sockaddr_in->sin_addr), ip4, INET_ADDRSTRLEN); // network to presentation

      std::cout << "Dirección IPv4 del cliente: " << ip4 << std::endl;
      std::cout << "Puerto del cliente: " << ntohs(temp_sockaddr_in->sin_port) << std::endl;
    }
  else
    {
      // significa que mi cliente usa IPv6
      // casteamos their_addr a sockaddr_in6 (para IPv6)
      struct sockaddr_in6 *temp_sockaddr_in6 = (struct sockaddr_in6 *)their_addr;
      char ip6[INET6_ADDRSTRLEN];                                                  // space to hold the IPv6 string
      inet_ntop(AF_INET6, &(temp_sockaddr_in6->sin6_addr), ip6, INET6_ADDRSTRLEN); // network to presentation

      std::cout << "Dirección IPv6 del cliente: " << ip6 << std::endl;
      std::cout << "Puerto del cliente: " << ntohs(temp_sockaddr_in6->sin6_port) << std::endl;
    }
}

// incluye toda la información de IP y Puerto en nuestra estructura *res
void setaddressinfo(char* PORT, struct addrinfo*& res){
  int status;
  
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     // use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // para que el programa complete mi propio IP

  // validando la función getaddrinfo
  // llenaremos toda la información relevante en nuestra estructura res, con los datos
  // de nuestro servidor local, p.ej la IP local y el puerto 45000 en este caso
  if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0)
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }
}

// creará el socket principal y lo pondrá a escuchar por conexiones entrantes
void setmainsock(struct addrinfo *res, int &sockfd, int BACKLOG = 10)
{
  // make a socket
  // int socket(int domain, int type, int protocol) : (AF_INET, SOCK_STREAM, IPPROTO_TCP)
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if (sockfd == -1){
    printf("error creating socket");
    exit(1);
  }

  int yes = 1;
  // para evitar el mensaje de error "Address already in use", cuando olvidamos cerrar el socket
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  {
    perror("setsockopt");
    exit(1);
  }

  // bind it to the port we passed in to getaddrinfo():
  // int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
  if (-1 == bind(sockfd, res->ai_addr, res->ai_addrlen))
    {
      perror("error bind failed");
      close(sockfd);
      exit(EXIT_FAILURE);
    }
  
  // imprimiremos el IP actual de nuestro servidor
  char ipaddr[INET_ADDRSTRLEN];
  struct sockaddr_in *sa = (struct sockaddr_in *)&res->ai_addr;
  inet_ntop(AF_INET, &(sa->sin_addr), ipaddr, INET_ADDRSTRLEN);
  std::cout << "Dirección IP del servidor: " << ipaddr << std::endl;


  // hasta aquí hemos usado la variable res, así que liberamos la memoria de esta lista
  freeaddrinfo(res);

  // listen acepta el socket principal del servidor
  // y el BACKLOG que representa la cantidad de usuarios que se permitirán en la
  // conexión total
  if (-1 == listen(sockfd, BACKLOG))
    {
      perror("error listen failed");
      close(sockfd);
      exit(EXIT_FAILURE);
    }


}




#endif