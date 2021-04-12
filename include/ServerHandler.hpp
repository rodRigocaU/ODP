#ifndef SERVER_HANDLER_HPP_
#define SERVER_HANDLER_HPP_
#include <string>
#include <vector>
#include "Utils.hpp"
#include "ParserModule.hpp"
#include "ConstructorMessage.hpp"
#include "Netconf.hpp"

using namespace std;

namespace odp
{
    struct User
    {
        int sockfd;
        std::string password;
    };

    class ServerHandler
    {
    private:
        std::unordered_map<std::string, User> ActiveUsers; // (username: <sockfd, password>)
        odp::BufferParser ServerParser;

    public:
        // en esa función lo que haremos será crear un nuevo thread para un nuevo usuario ya registrado,
        //y escucharemos todas sus peticiones
        void handleuser(const std::string &username)
        {
            odp::CommandType comm_type; // el tipo de comando AskList, etc
            User curr_user;             // current user
            curr_user.sockfd = ActiveUsers[username].second.sockfd;
            curr_user.password = ActiveUsers[username].second.password;

            char buffer_token[1];   // l,i,m,u,x,etc
            char buffer_header[15]; // I03110305SantistebanLeePeter -> pasar en string 110305
            char buffer_content[100000];

            std::vector<std::string> data; // el vector donde entrará la data
            std::string message;
            std::size_t sizem;

            int nbytes;

            while (true)
            {
                // siempre al inicio limpiamos el vector, ya que tiene la data del anterior read
                data.clear();

                nbytes = recv(usuario.sockfd, buffer_token, 1, 0);

                // el tamaño del header dependiendo del comando
                sizem = ServerParser.getHeaderSize(odp::SenderType::User, buffer_token[0]);

                // Aqui debemos conseguir las respuesra
                switch (ServerParser.getCommandType())
                {
                case odp::CommandType::AskList:
                {
                    // primero leemos el header
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';
                    message = buffer_header;

                    sizem = atoi(message) * 2; //6

                    nbytes = recv(curr_user.sockfd, buffer_header, size, 0);
                    message = buffer_header; //110503

                    sizem = ServerParser.getContentSize(message); //18 porque 11+05+03

                    nbytes = recv(curr_user.sockfd, buffer_content, size, 0);
                    message = buffer_content; //juliosantiestabanlui...

                    data = ServerParser.getContentInTokens(message);

                    message = odp::ConstructorMessage::buildMessage(data, 'I', odp::SenderType::User);

                    write(curr_user.sockfd, message.c_str(), message.size());
                    break;
                }
                case odp::CommandType::UserMessage:
                {
                    //ejemplo

                    //OE PERO NO EXISTE DESYINATARIO EN EL SERVIDOR A USUARIO
                    /*
                    char accion;//M
                    char tamaño_msg[3];
                    char tamaño_remitente[2];
                    char * msg;
                    char * remitente;
                    */   
                   // julio envía un "hola" a pancho
                   // protocolo: m00405holapancho
                    // leemos el header
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';
                    message = buffer_header;
                    // leemos el contenido
                    sizem = ServerParser.getContentSize(message);
                    nbytes = recv(curr_user.sockfd, buffer_content, sizem, 0);
                    buffer_content[nbytes] = '\0';
                    message = buffer_content;
                    data = ServerParser.getContentInTokens(message); //data_body y destinatario

                    std::string destinatario = data[1]; //obtener el destinatario B

                    // enviamos esto a pancho: M00405holajulio
                    message = odp::ConstructorMessage::buildMessage(std::vector<std::string>({data[0], username}), 'M', odp::SenderType::User);
                    // enviamos el mensaje al socketfd del destinatario
                    write(ActiveUsers[destinatario].sockfd, message.c_str(), message.size());

                    break;
                }
                case odp::CommandType::BroadcastMessage:
                {
                    /*
                    [Server ----> user]
                    {
                    char accion;//B
                    char tamaño_msg[3];
                    char tamaño_remitente[2];
                    char * msg;
                    char * remitente;
                    }
                    */

                    // EJEMPLO [user->server]
                    // b004Hola
                    
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';

                    message = buffer_header; //004
                    // obtenemos el tamaño del content
                    sizem = ServerParser.getContentSize(message); //4

                    // leemos el content
                    nbytes = recv(curr_user.sockfd, buffer_content, sizem, 0);
                    buffer_content[nbytes] = '\0';
                    message = buffer_content; 
                    // data = [msg] = [hola]
                    data = ServerParser.getContentInTokens(message);
                    
                    // B00405holajulio
                    message = odp::ConstructorMessage::buildMessage(std::vector<std::string>({data[0], username}), 'B', odp::SenderType::User);
                    
                    for(auto &User : ActiveUsers){
                        write(User.second.sockfd, message.c_str(), message.size());
                    }
                    break;
                }
                case odp::CommandType::UploadFile:
                {
                    
                    //EJEMPLO [user->server]
                    /*
                    char accion;/U
                    char tamaño_file_name[3];
                    char tamaño_file_data[10];
                    char tamaño_remitente[2];
                    char * file_name;
                    char * file_data;
                    char * remitente;
                    }*/
                    // julio envia un file a pancho, el file se llama [hola.txt] y su contenido es "hola_pancho"
                    // ejemplo: u008000000001005hola.txthola_panchopancho
                    
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';

                    message = buffer_header; //008000000001005
                    // obtenemos el tamaño del content
                    sizem = ServerParser.getContentSize(message); // 8+10+5=23
                    // leemos el contenido
                    nbytes = recv(curr_user.sockfd, buffer_content, sizem, 0);// hola.txthola_panchopancho
                    buffer_content[nbytes] = '\0';
                    message = buffer_content;
                    // data = [hola.txt,hola_pancho,pancho]
                    data = ServerParser.getContentInTokens(message);
                    std::string destinatario = data[2];// pancho
                    // ahora contruimos el message
                    // U008000000001005hola.txthola_panchojulio
                    message = odp::ConstructorMessage::buildMessage(std::vector<std::string>({data[0], data[1], username}), 'U', odp::SenderType::User);
                    
                    write(ActiveUsers[destinatario].sockfd, message.c_str(), message.size());
                    
                    break;
                }
                case odp::CommandType::AcceptFile:
                {
                    /*{
                    char acccion;//F  se rechaso el archivo 
                    char tamaño_user_name[2]; 
                    char * user_name;
                    }*/
                    // singlefile
                    // julio envió un archivo a pancho, pero pancho no lo aceptó, entonces le envía un mensaje de rechazo
                    // ejemplo: f05julio
                    // primero leemos el header
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);// 05
                    buffer_header[nbytes] = '\0';
                    message = buffer_header;
                    // ahora obtenemos el tamaño del contenido
                    sizem = ServerParser.getContentSize(message); // 5, por "julio"
                    // leemos el contenido
                    nbytes = recv(curr_user.sockfd, buffer_content, sizem, 0);// julio
                    buffer_content[sizem] = '\0';
                    message = buffer_content;
                    // data = [julio]
                    data = ServerParser.getContentInTokens(message);
                    // message = F05pancho
                    message = odp::ConstructorMessage::buildMessage(std::vector<std::string>({username}), 'F', odp::SenderType::User);
                    write(ActiveUsers[data[0]].sockfd, message.c_str(), message.size());
                    break;
                }
                case odp::CommandType::Exit:
                {
                    // julio envía el mensaje de salida al usuario
                    break;
                }
                case odp::CommandType::None:
                    cout << "not known message in protocol" << endl;
                    break;
                }
            }
            close(usuario.sockfd);
        }

        // registra un nuevo usuario por medio del nuevo socket aceptado en el servidor
        // retorna vacío si no se pudo registrar un nuevo usuario
        // retorna el nombre del usuario si se pudo registrar
        string registeruser(int &connectfd)
        {
            uint32_t nbytes;
            char buffer_token[1];  // l,i,m,u,x,etc
            char buffer_header[4]; // I03110305SantistebanLeePeter -> pasar en string 110305
            char buffer_content[200];

            // Aplicación
            // lo primero en el protocolo es recibir la letra "l" de parte del usuario

            // Leer Token
            nbytes = recv(connectfd, buffer_token, 1, 0);
            std::string message;

            // Leer size del header
            size_t sizem = ServerParser.getHeaderSize(odp::SenderType::User, buffer_token[0]);
            nbytes = recv(connectfd, buffer_header, sizem, 0);
            message = buffer_header;

            // leer cuerpo del mensaje
            sizem = ServerParser.getContentSize(message);
            nbytes = recv(connectfd, buffer_content, sizem, 0);
            buffercontent[nbytes] = '\0';
            message = buffer_content;
            // newuserdata = [<username>, <password>]
            std::vector<std::string> newuserdata = ServerParser.getContentInTokens(message);

            /* aquí podría iniciarse la función registrar()*/
            /* si el usuario no está registrado lo registramos */
            if (ActiveUsers.find(newuserdata[0]) != ActiveUsers.end())
            {
                User usuario;
                usuario.sockfd = connectfd;
                usuario.password = newuserdata[1];
                users[newuserdata[0]] = usuario;
                return newuserdata[0];
            }
            else
            {
                nbytes = send(connectfd, "", 7, 0);
                shutdown(connectfd, SHUT_RDWR);
                close(connectfd);
                return "";
            }
        }
        ~ServerHandler()
        {
            ActiveUsers.clear();
        }
    };

}

#endif // SERVER_HANDLER_HPP_