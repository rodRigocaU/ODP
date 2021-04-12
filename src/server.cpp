#include "ParserModule.hpp"
#include "Netconf.hpp"
#include "Utils.hpp"
#include <thread>
#include <utility>
#include <string>

using namespace std;

struct User{
  int sockfd;
  string password;
};

std::unordered_map <std::string, User> users; // (username: <sockfd, password>)

// en esa función lo que haremos será crear un nuevo thread para un nuevo usuario ya registrado,
//y escucharemos todas sus peticiones
void handleuser(const string& username){
  odp::CommandType comm_type;// el tipo de funcionalidad
  std::vector<std::string> data; // el vector donde entrará la data
  string message;
  User usuario = users[username];
  int nbytes, sizem;
  char buffer[MAXSIZE];
  while(true){
    // primero recibimos el tamaño del mensaje, por defecto leemos los primeros 6 bytes
    nbytes = recv(usuario.sockfd, buffer, 6, 0);
    buffer[nbytes] = '\0';
    message = buffer;
    sizem = strtoi(message);
    
    switch(comm_type = odp::ParserODP::ProcessBuffer(odp::SenderType::User, message, data)){
    case odp::AskList:
      
      break;
    case odp::UserMessage:
      break;
    case odp::BroadcastMessage:
      break;
    case odp::UploadFile:
      
      break;
    case odp::BroadcastFile:
      nbytes = send(usuario.sockfd, "FA#", 3, 0);
      // por ahora no validaré los envíos
      // recibiremos la respuesta del usuario, yes o no, para aceptar el file
      nbytes = recv(usuario.sockfd, buffer, MAXSIZE, 0);
      buffer[nbytes] = '\0';
      message = buffer;
      vector<string> ans;// aquí guardamos su respuesta
      // parseamos la respuesta del usuario, esto es así para que en el protocolo se vea
      // bien presentado el esquema de envío de archivos
      odp::ParserODP::ProcessBuffer(odp::SenderType::User, message, ans); 
      
      
      break;
    case odp::AcceptFile:
      // broadcastfile
      
      // singlefile
      
	break;
    case odp::Exit:
      break;
    case odp::None:
      cout << "not known message in protocol" << endl;
      break;
    }
  }
  close(usuario.sockfd);
}


int main()
{
  //EXAMPLE:
  //clog::ConsoleOutput::enableOutput(0); //set 0 if you want omit the outputs of the parser
   std::cout << "im server\n";
  //EXAMPLE:
  //clog::ConsoleOutput::enableOutput(0); //set 0 if you want omit the outputs of the parser
 
  std::vector<std::string> rawData;
  odp::CommandType comm_type;
  
  struct sockaddr_storage their_addr; // se utilizará en la función connect()
  socklen_t addr_size;                // se utilizará en la función connect()

    struct addrinfo *res;

    setaddressinfo("50000", res);

    int sockfd;
    setmainsock(res, sockfd);


    addr_size = sizeof their_addr;
    int nbytes;
    char buffertoken[1]; // el buffer es de 999, ya que el mensaje
    char bufferheader[4];
    char buffercontent[99+99];
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
      
      // Aplicación
	    // lo primero en el protocolo es recibir la letra "l" de parte del usuario
      nbytes = recv(connectfd, buffertoken, 1, 0);
      string message(buffertoken);
      size_t sizem = odp::getsizeheader(message);
	    nbytes = recv(connectfd, bufferheader, sizem, 0);
	    message = bufferheader;
	    sizem = odp::getsizecontent(message);
	    nbytes = recv(connect, buffercontent, sizem, 0);
	    buffercontent[nbytes] = '\0';
	    message = buffer;
	    vector<string> newuserdata = odp::readbuffer(message);

	
	    /* aquí podría iniciarse la función registrar()*/
	    /* si el usuario no está registrado lo registramos */
	    if(users.find(newuserdata[0]) != users.end()){
        User usuario;
	      usuario.sockfd = connectfd;
	      usuario.password = newuserdata[1];
	      users[newuserdata[0]] = usuario;
	      thread(handleuser, newuserdata[0]).detach();
	    }
	    else{
	      nbytes = send(connectfd, "", 7, 0);
	      close(connectfd);
	    }
    }
    close(sockfd);
    return 0;
}
