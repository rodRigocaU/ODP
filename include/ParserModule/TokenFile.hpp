#ifndef TOKEN_FILE_HPP_
#define TOKEN_FILE_HPP_

frozonus
#define TOKEN_COMM_LOGIN                  'l'
#define TOKEN_COMM_LIST                   'i'
#define TOKEN_COMM_MESSAGE_USER           'm'
#define TOKEN_COMM_MESSAGE_BROADCAST      'b'
#define TOKEN_COMM_UPLOAD_FILE            'u'
#define TOKEN_COMM_ACCEPT_FILE            'f'
#define TOKEN_COMM_EXIT                   'x'
#define TOKEN_COMM_ERROR                  'e'



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
frozonus
  BroadcastFile,
carpincho
  AcceptFile,
  Exit,
  Error,
  None
};

}

frozonus
#endif//TOKEN_FILE_HPP_

