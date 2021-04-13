#ifndef TOKEN_FILE_HPP_
#define TOKEN_FILE_HPP_

#define TOKEN_COMM_LOGIN                  'l'
#define TOKEN_COMM_LIST                   'i'
#define TOKEN_COMM_MESSAGE_USER           'm'
#define TOKEN_COMM_MESSAGE_BROADCAST      'b'
#define TOKEN_COMM_UPLOAD_FILE            'u'
#define TOKEN_COMM_ACCEPT_FILE            'f'
#define TOKEN_COMM_EXIT                   'x'
#define TOKEN_COMM_ERROR                  'e'

#define ERROR_MESSAGE_LOGIN               "error_al_ingresar___"
#define ERROR_MESSAGE_LOGOUT              "error_al_salir______"
#define ERROR_USER_NOT_FOUND              "error_no_hay_usuario"
#define ERROR_MESSAGE_NOT_IN_PROTOCOL     "error_mensaje_malo__"


namespace odp{

enum SenderType{
  User, Server
};

enum CommandType{
  Login,
  AskList,
  UserMessage,
  BroadcastMessage,
  UploadFile,
  BroadcastFile,
  AcceptFile,
  Exit,
  Error,
  None
};

}

#endif//TOKEN_FILE_HPP_

