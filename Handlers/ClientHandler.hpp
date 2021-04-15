#ifndef CLIENT_HANDLER_HPP_
#define CLIENT_HANDLER_HPP_

#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define REDB "\e[41m"

#include "../ParserModule.hpp"
#include "../ParserModule/ConstructorMessage.hpp"
#include "Netconf.hpp"
#include <fstream>

namespace odp
{
    class ClientHandler
    {
    private:
        std::string username;
        std::string password;
        int sockfd;
        BufferParser ClientParser;

    public:
        ClientHandler(int _sockfd, const std::string &_username, const std::string &_password)
        {
            username = _username;
            password = _password;
            sockfd = _sockfd;
        }

        void handlerecv()
        {

            char buffer_token[2];           // l,i,m,u,x,etc
            char buffer_header[99 * 2 + 2]; // I03110305SantistebanLeePeter -> pasar en string 110305
            char buffer_content[100000];

            std::vector<std::string> data, send_data; // el vector donde entrará la data
            std::string message;
            std::size_t sizem; // [0, MAXSIZE]
            ssize_t nbytes;    // [-1, MAXSIZE]

            while (true)
            {
                // siempre lo primero que se hace es limpiar el vector que contendrá la data
                data.clear();
                send_data.clear();
                message.clear();
                nbytes = recv(sockfd, buffer_token, 1, 0);

                buffer_token[1] = '\0';
                //--------------------------------
                std::cout << "Bytes leidos desde el cliente: " << nbytes << '\n';
                std::cout << "Bytes leidos desde el cliente: " << nbytes << '\n';
                std::cout << "Token: " << buffer_token << '\n';
                //-----------------------------------------------------------

                // el tamaño del header dependiendo del comando
                sizem = ClientParser.getHeaderSize(odp::SenderType::Server, buffer_token[0]);

                //-------------------------------------------------------------
                std::cout << "Size del header: " << sizem << '\n';
                //-------------------------------------------------------------

                switch (ClientParser.getCommandType())
                {
                case odp::CommandType::AskList:
                {
                    clog::ConsoleOutput::print("Entramos AskList ");
                    // supongamos que nos llega el mensaje: I03110305SantistebanLeePeter
                    // primero leemos el header
                    nbytes = recv(sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';

                    clog::ConsoleOutput::print("Segunda lectura");
                    clog::ConsoleOutput::print(buffer_header);

                    message = buffer_header;
                    //-------------------------------------------
                    std::cout << message << std::endl;
                    //--------------------------------------------

                    // stringstream ss;
                    // ss << message;
                    // ss >> sizem;
                    //Esta bien cagada
                    sizem = std::stoi(message) * 2; //6
                    std::cout << sizem << std::endl;

                    nbytes = recv(sockfd, buffer_header, sizem, 0);
                    message = message + buffer_header; //110503

                    // clog::ConsoleOutput::print("Mensaje: ");
                    // clog::ConsoleOutput::print(buffer_header);
                    std::cout << message << std::endl;

                    //Enviar concatenado 0104... no body
                    sizem = ClientParser.getContentSize(message); //18 = 11+05+03

                    nbytes = recv(sockfd, buffer_content, sizem, 0);
                    message = buffer_content; //juliosantiestabanlui...

                    data = ClientParser.getContentInTokens(message);
                    // presentamos la información
                    std::cout << "\nLista de Usuarios en el chat:\n";
                    // recorremos toda la lista de usuarios
                    for (auto &user : data)
                    {
                        std::cout << user << std::endl;
                    }
                    std::cout << "\n";
                    break;
                }
                case odp::CommandType::UserMessage:
                {
                    // si recibo un mensaje de julio, este sería: M00405holajulio
                    nbytes = recv(sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';
                    message = buffer_header;
                    // obtenemos el tamaño del contenido
                    sizem = ClientParser.getContentSize(message);
                    nbytes = recv(sockfd, buffer_content, sizem, 0);
                    buffer_content[nbytes] = '\0';
                    message = buffer_content;
                    // extraemos los datos
                    data = ClientParser.getContentInTokens(message);
                    // presentamos la data
                    // data = [mensaje, remitente]
                    std::cout << "\nMensaje nuevo de [" << data[1] << "]:\n";
                    std::cout << data[0] << "\n";
                    break;
                }
                case odp::CommandType::BroadcastMessage:
                {
                    // EJEMPLO [server->user]
                    // B00405Holajulio

                    nbytes = recv(sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';

                    message = buffer_header; //00405
                    // obtenemos el tamaño del content
                    sizem = ClientParser.getContentSize(message); //9

                    // leemos el contenido
                    nbytes = recv(sockfd, buffer_content, sizem, 0);
                    buffer_content[nbytes] = '\0';
                    message = buffer_content;
                    // data = [Hola,julio]
                    data = ClientParser.getContentInTokens(message);
                    // presentamos la información
                    // data = [mensaje, remitente]
                    std::cout << "\nMensaje global de[" << data[1] << "]:\n";
                    std::cout << data[0] << "\n";
                    break;
                }
                case odp::CommandType::UploadFile:
                {
                    // julio nos envió un archivo llamado [hola.txt] y su contenido es "hola_pancho"
                    // ejemplo: u008000000001005hola.txthola_panchojulio

                    nbytes = recv(sockfd, buffer_header, sizem, 0);
                    buffer_header[nbytes] = '\0';

                    message = buffer_header; //008000000001005
                    // obtenemos el tamaño del content
                    sizem = ClientParser.getContentSize(message); // 8+10+5=23
                    // leemos el contenido
                    nbytes = recv(sockfd, buffer_content, sizem, 0); // hola.txthola_panchojulio
                    buffer_content[nbytes] = '\0';
                    message = buffer_content;
                    // data = [hola.txt,hola_pancho,julio]
                    data = ClientParser.getContentInTokens(message);

                    char accept; // char para aceptar el archivo
                    std::cout << "\nNuevo archivo \"" << data[0] << "\" de [" << data[2] << "] aceptar [y/n]?: ";
                    std::cin >> accept;
                    if (accept == 'y')
                    {
                        std::ofstream newFile;
                        newFile.open(data[0]);
                        newFile << data[1];
                        newFile.close();
                        std::cout << "\nNuevo archivo \"" << data[0] << "\" escrito.\n";
                    }
                    else
                    {
                        send_data.push_back(username);
                        // si el archivo fué rechazado, enviamos el mensaje: f05pancho
                        message = odp::ConstructorMessage::buildMessage(send_data, 'f', odp::SenderType::User);
                        send(sockfd, message.c_str(), message.size(), 0); // enviamos el mensaje de error
                    }
                    break;
                }
                case odp::CommandType::AcceptFile:
                {
                    // singlefile
                    // enviaste un archivo a julio, pero julio no lo aceptó
                    // ejemplo: f05julio
                    // primero leemos el header
                    nbytes = recv(sockfd, buffer_header, sizem, 0); // 05
                    buffer_header[nbytes] = '\0';
                    message = buffer_header;
                    // ahora obtenemos el tamaño del contenido
                    sizem = ClientParser.getContentSize(message); // 5, por "julio"
                    // leemos el contenido
                    nbytes = recv(sockfd, buffer_content, sizem, 0); // julio
                    buffer_content[sizem] = '\0';
                    message = buffer_content;
                    // data = [julio]
                    data = ClientParser.getContentInTokens(message);
                    std::cout << "[" << data[0] << "] no aceptó tu archivo\n";
                    break;
                }
                case odp::CommandType::Exit:
                {
                    // el server aceptó tu retiro del chat, X
                    close(sockfd); // cerramos nuestro socket
                    exit(1);       // finalizamos todo el programa
                    break;
                }
                case odp::CommandType::Error:
                {
                    nbytes = recv(sockfd, buffer_content, 20, 0);
                    buffer_content[nbytes] = '\0';
                    std::cout << "\nNuevo error: " << buffer_content << "\n";
                    break;
                }
                }
            }
        }

        void help()
        {

            std::cout << "Comandos de Ayuda:";
            std::cout << '\n';

            std::cout << CYN "Login:"
                      << " ";
            std::cout << CYN "l -> lnamepassword" << '\n';

            std::cout << CYN "Lista de usuarios:"
                      << " ";
            std::cout << CYN "i" << '\n';

            std::cout << CYN "Mensaje a usuario en concreto:"
                      << " ";
            std::cout << CYN "m -> mMensajedestinatario" << '\n';

            std::cout << CYN "Mensaje a todos los usuarios:"
                      << " ";
            std::cout << CYN "b -> bmensajebroadcast" << '\n';

            std::cout << CYN "Subir un archivo al servidor:"
                      << " ";
            std::cout << CYN "u -> unombredelarchivoarchivodestinatario" << '\n';

            std::cout << CYN "Aceptacion del archivo:"
                      << " ";
            std::cout << CYN "f -> fremitente" << '\n';

            std::cout << RED "Salir del servidor:"
                      << " ";
            std::cout << RED "x" NC << '\n';
        }

        void handlesend()
        {

            help();
            std::string message;
            ssize_t nbytes;
            while (true)
            {
                message.clear();

                std::getline(std::cin, message);

                if (message == "help")
                    help();
                else
                {
                    //Hacer que pida el comando luego respectivamente que pida los cuertpos de los mensajes no pedira el tamaño la funcio nde italo nos dara el mensaje concatenado y listo

                    int n = write(sockfd, message.c_str(), message.length());

                    // clog::ConsoleOutput::print("Bytes enviados:");
                    // std::cout << n << std::endl;
                }
            }
        }
    };

}

#endif //CLIENT_HANDLER_HPP_