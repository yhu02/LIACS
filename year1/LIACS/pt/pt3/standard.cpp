#include "standard.h"

//Read character from cin
char readChar()
{
  while(true){
    std::string trash = "";
    char character = std::cin.get();
    
    getline(std::cin,trash);
    std::cin.clear();
  
    if(!trash.empty()){
      return 'X';
    }
    return character;
  }
};