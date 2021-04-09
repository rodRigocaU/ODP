#ifndef TOKEN_FILE_HPP_
#define TOKEN_FILE_HPP_

#define TOKEN_SEPARATOR           '#'
#define TOKEN_COMM_LOGIN          "lo"
#define TOKEN_COMM_LIST           "lu"
#define TOKEN_COMM_MESSUSER       "mu"
#define TOKEN_COMM_MESSBRC        "mb"
#define TOKEN_COMM_FILEUSER       "fu"
#define TOKEN_COMM_FILEBRC        "fb"
#define TOKEN_COMM_FILEACCEPT     "fa"
#define TOKEN_COMM_EXIT           "x"
#define TOKEN_COMM_ERROR          "err"


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