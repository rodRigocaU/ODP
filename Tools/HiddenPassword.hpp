#ifndef HIDDEN_PASSWORD_HPP_
#define HIDDEN_PASSWORD_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <string>
#include <iostream>

#define MAXPW 99


namespace odp{

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void getPassword(std::string& password){
  char pw;
  while((pw = getch()) != '\n'){
    if(pw == char(127)){
      password.pop_back();
      std::cout << "\b \b";
    }
    else{
      password.push_back(pw);
      std::cout << '*';
    }
  }std::cout << std::endl;
}

}
#endif//HIDDEN_PASSWORD_HPP_