#ifndef SERVER_HANDLER_HPP_
#define SERVER_HANDLER_HPP_
#include <string>
#include <vector>
#include "ParserModule.hpp"
#include "ConstructorMessage.hpp"
#include "Netconf.hpp"

using namespace std;

namespace odp
{
    struct ActiveUser
    {
        int sockfd;
        std::string password;
    };

    class ServerHandler
    {
    private:
        std::string username;
        std::unordered_map<std::string, ActiveUser> ActiveUsers; // (username: <sockfd, password>)
        BufferParser ServerParser;

    public:
        // en esa función lo que haremos será crear un nuevo thread para un nuevo usuario ya registrado,
        //y escucharemos todas sus peticiones
        void handleuser(int sockfd)
        {
            ActiveUser curr_user; // current user

            char buffer_token[1];           // l,i,m,u,x,etc
            char buffer_header[99 * 2 + 2]; // I03110305SantistebanLeePeter -> pasar en string 110305
            char buffer_content[100000];

            std::vector<std::string> data, send_data; // el vector donde entrará la data
            std::string message;
            std::size_t sizem;   // [0, MAXSIZE]
            ssize_t nbytes; // [-1, MAXSIZE]

            while (true)
            {
                // siempre al inicio limpiamos el vector y el mensaje, ya que tienen datos de lecturas anteriores
                data.clear();
                send_data.clear();
                message.clear();

                nbytes = recv(sockfd, buffer_token, 1, 0);

                if (nbytes == 0)
                {
                    shutdown(sockfd, SHUT_RDWR);
                    close(sockfd);
                    return;
                }

                // el tamaño del header dependiendo del comando
                sizem = ServerParser.getHeaderSize(odp::SenderType::User, buffer_token[0]);

                // Aqui debemos conseguir las respuesra
                switch (ServerParser.getCommandType())
                {
                case odp::CommandType::Login:
                {
                    // Leer el header
                    nbytes = recv(sockfd, buffer_header, sizem, 0);
                    message = buffer_header;

                    // leer cuerpo del mensaje
                    sizem = ServerParser.getContentSize(message);
                    nbytes = recv(sockfd, buffer_content, sizem, 0);
                    buffer_content[nbytes] = '\0';
                    message = buffer_content;

                    // newuserdata = [<username>, <password>]
                    data = ServerParser.getContentInTokens(message);

                    /* aquí podría iniciarse la función registrar()*/
                    /* si el usuario no está registrado lo registramos */
                    if (ActiveUsers.find(data[0]) != ActiveUsers.end())
                    {
                        curr_user.sockfd = sockfd;
                        curr_user.password = data[1];
                        ActiveUsers[data[0]] = curr_user;
                        send(curr_user.sockfd, "Lok", 3, 0);
                    }
                    else
                    {
                        std::vector<std::string> err({ERROR_MESSAGE_LOGIN});
                        message = odp::ConstructorMessage::buildMessage(err, 'E', odp::SenderType::User);
                        send(curr_user.sockfd, message.c_str(), message.size(), 0);
                    }
                    break;
                }
                case odp::CommandType::AskList:
                {
                    // agregamos a data todos los nombres de los usuarios
                    for (auto &activeuser : ActiveUsers)
                    {
                        data.push_back(activeuser.first);
                    }

                    message = odp::ConstructorMessage::buildMessage(data, 'I', odp::SenderType::User);

                    write(curr_user.sockfd, message.c_str(), message.size());
                    break;
                }
                case odp::CommandType::UserMessage:
                {
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

                    // validamos la existencia del usuario
                    if (ActiveUsers.find(destinatario) != ActiveUsers.end())
                    {
                        // el usuario existe
                        // enviamos esto a pancho: M00405holajulio
                        std::vector<std::string> send_data({data[0], username});
                        message = odp::ConstructorMessage::buildMessage(send_data, 'M', odp::SenderType::User);
                        // enviamos el mensaje al sockfd del destinatario
                        write(ActiveUsers[destinatario].sockfd, message.c_str(), message.size());
                    }
                    else
                    {
                        // el usuario no existe, error
                        std::vector<std::string> err({data[0], username});
                        message = odp::ConstructorMessage::buildMessage(err, 'E', odp::SenderType::User);
                        send(curr_user.sockfd, message.c_str(), message.size(), 0);
                    }
                    break;
                }
                case odp::CommandType::BroadcastMessage:
                {
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
                    std::vector<std::string> send_data({data[0], username});
                    message = odp::ConstructorMessage::buildMessage(send_data, 'B', odp::SenderType::User);

                    // enviamos el mensaje a todos los usuarios, excepto al que lo envió
                    for (auto &User : ActiveUsers)
                    {
                        if (User.first != username)
                            write(User.second.sockfd, message.c_str(), message.size());
                    }
                    break;
                }
                case odp::CommandType::UploadFile:
                {
                    // julio envia un file a pancho, el file se llama [hola.txt] y su contenido es "hola_pancho"
                    // ejemplo: u008000000001005hola.txthola_panchopancho

                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';

                    message = buffer_header; //008000000001005
                    // obtenemos el tamaño del content
                    sizem = ServerParser.getContentSize(message); // 8+10+5=23
                    // leemos el contenido
                    nbytes = recv(curr_user.sockfd, buffer_content, sizem, 0); // hola.txthola_panchopancho
                    buffer_content[nbytes] = '\0';
                    message = buffer_content;
                    // data = [hola.txt,hola_pancho,pancho]
                    data = ServerParser.getContentInTokens(message);
                    std::string destinatario = data[2]; // pancho

                    // validamos la existencia del usuario
                    if (ActiveUsers.find(destinatario) != ActiveUsers.end())
                    {
                        // el usuario existe
                        // ahora contruimos el message
                        // U008000000001005hola.txthola_panchojulio
                        std::vector<std::string> send_data({data[0], data[1], username});
                        message = odp::ConstructorMessage::buildMessage(send_data, 'U', odp::SenderType::User);
                        send(ActiveUsers[destinatario].sockfd, message.c_str(), message.size(), 0);
                    }
                    else
                    {
                        // el usuario no existe, error
                        std::vector<std::string> err({ERROR_USER_NOT_FOUND});
                        message = odp::ConstructorMessage::buildMessage(err, 'E', odp::SenderType::User);
                        send(curr_user.sockfd, message.c_str(), message.size(), 0);
                    }
                    break;
                }
                case odp::CommandType::AcceptFile:
                {
                    // singlefile
                    // julio envió un archivo a pancho, pero pancho no lo aceptó, entonces le envía un mensaje de rechazo
                    // ejemplo: f05julio
                    // primero leemos el header
                    nbytes = recv(curr_user.sockfd, buffer_header, sizem, 0); // 05
                    buffer_header[nbytes] = '\0';
                    message = buffer_header;
                    // ahora obtenemos el tamaño del contenido
                    sizem = ServerParser.getContentSize(message); // 5, por "julio"
                    // leemos el contenido
                    nbytes = recv(curr_user.sockfd, buffer_content, sizem, 0); // julio
                    buffer_content[sizem] = '\0';
                    message = buffer_content;
                    // data = [julio]
                    data = ServerParser.getContentInTokens(message);
                    // message = F05pancho
                    std::vector<std::string> send_data({username});
                    message = odp::ConstructorMessage::buildMessage(send_data, 'F', odp::SenderType::User);
                    write(ActiveUsers[data[0]].sockfd, message.c_str(), message.size());
                    break;
                }
                case odp::CommandType::Exit:
                {
                    // julio envía el mensaje de salida al usuario
                    // x
                    ActiveUsers.erase(username);       // eliminamos al usuario de la estructura
                    send(curr_user.sockfd, "X", 1, 0); // solo enviamos una X al usuario
                    close(curr_user.sockfd);
                    return; // finalizamos la función
                    break;
                }
                case odp::CommandType::None:
                {
                    std::vector<std::string> err({ERROR_MESSAGE_NOT_IN_PROTOCOL});
                    message = odp::ConstructorMessage::buildMessage(err, 'E', odp::SenderType::User);
                    // enviamos un error
                    send(curr_user.sockfd, message.c_str(), message.size(), 0);
                    break;
                }
                default:
                close(curr_user.sockfd);
                return;
                }
            }
        }

        ~ServerHandler()
        {
            ActiveUsers.clear();
        }
    };

}

#endif // SERVER_HANDLER_HPP_
