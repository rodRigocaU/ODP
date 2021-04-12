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
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';
                    message=buffer_header;

                    sizem = atoi(message) * 2; //6

                    nbytes = recv(curr_user.sockfd, buffer_header, size, 0);
                    message=buffer_header; //110503

                    sizem = ServerParser.getContentSize(message); //18 porque 11+05+03

                    nbytes = recv(curr_user.sockfd, buffer_content, size, 0);
                    message = buffer_content; //juliosantiestabanlui...

                    data = ServerParser.getContentInTokens(message);

                    message = odp::ConstructorMessage::buildMessage(data,'I',odp::SenderType::User);
                    
                    write(curr_user.sockfd, message.c_str(), message.size());
                    break;
                }
                case odp::CommandType::UserMessage:
                    // leemos el header
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';
                    message = buffer_header;
                    // leemos el contenido
                    sizem = ServerParser.getContentSize(message);
                    nbytes = recv(curr_user.sockfd, buffer_content, sizem, 0);
                    buffer[nbytes] = '\0';
                    message = buffer;
                    data = ServerParser.getContentInTokens(message);
                    break;
                case odp::CommandType::BroadcastMessage:

                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';

                    string Messa(buffer_header);

                    std::size_t size1 = atoi(Messa); //004

                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);

                    string Remi(buffer_header);
                    std::size_t size2 = atoi(Remi);

                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);

                    string AllMessa(buffer_header);

                    
                    break;
                case odp::CommandType::UploadFile:
                    

                    break;
                case odp::CommandType::AcceptFile:
                    // broadcastfile

                    // singlefile

                    break;
                case odp::CommandType::Exit:
                    break;
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