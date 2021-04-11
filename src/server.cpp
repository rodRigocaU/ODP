#include "ParserModule.hpp"
#include "Netconf.hpp"
#include "Utils.hpp"
#include <thread>
#include <map>
#include <utility>
#include <string>

#define MAXSIZE 100210 // el mensaje máximo posible del protocolo es 100206

using namespace std;

struct User{
  int sockfd;
  string password;
};

map <std::string, User> users; // (username: <sockfd, password>)

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
    char buffer[MAXSIZE]; // el buffer es de 999, ya que el mensaje
    string msg;
    User usuario;
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

	// lo primero en el protocolo es recibir su nombre de usuario
	// y constraseña, obligadamente, para recién poder lanzar el thread
	nbytes = recv(connectfd, buffer, MAXSIZE, 0);
	buffer[nbytes] = '\0';
	msg = buffer;
	// lo primero es parsear el nombre de usuario y el password
	comm_type = odp::ParserODP::ProcessBuffer(odp::SenderType::User, msg, rawData);
	
	/* aquí podría iniciarse la función registrar()*/
	/* si el usuario no está registrado lo registramos */
	if(users.find(rawData[0]) != users.end()){
	  usuario.sockfd = connectfd;
	  usuario.password = rawData[1];
	  users[rawData[0]] = usuario;
	  thread().detach();
	}
	else{
	  nbytes = send(connectfd, "ERR#2LO", 7, 0);
	  close(connectfd);
	}
    }
    close(sockfd);
    return 0;
}
