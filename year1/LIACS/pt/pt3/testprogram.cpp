#include "testprogram.h"
//Main menu
TestProgram::TestProgram()
{
  std::cout << "This is the test program" << std::endl;
  while(true)
  {
    int choice = menu(0);

    if(choice == 0)
    {
      StringSeq seq1(0);
      
    }else if(choice == 1){
      int choice2 = menu(1);
      if(choice2 == 0)
      {
        ArraySeq<u_int8_t>  seq1(0);
      }else if(choice2 == 1){
        ArraySeq<u_int16_t> seq1(0);
      }else if(choice2 == 2){
        ArraySeq<u_int32_t> seq1(0);
      }else if(choice2 == 3){
        ArraySeq<u_int64_t> seq1(0);
      }

    }else if(choice == 2){
      int choice2 = menu(1);
      if(choice2 == 0)
      {
        DLinkedListSeq<u_int8_t>  seq1(0);
      }else if(choice2 == 1){
        DLinkedListSeq<u_int16_t> seq1(0);
      }else if(choice2 == 2){
        DLinkedListSeq<u_int32_t> seq1(0);
      }else if(choice2 == 3){
        DLinkedListSeq<u_int64_t> seq1(0);
      }
    }
  }
};

//Constructor
TestProgram::~TestProgram()
{

};

//Check if file can be opened
std::string TestProgram::read()
{
  std::ifstream input;      
  std::string input_name;

  do
  {
    std::cout << "Type the name of the file containing the variants" << std::endl;
    std::cin >> input_name;
    input.open(input_name,std::ios::in);
    if(input.fail())
    {
      std::cout << "File doesn't exist\n";
    }else{
      input.close();
      break;
    }
  } while(true);

  return input_name;
}


int TestProgram::menu(int menuType)
{
  while(true)
  {
    if(menuType == 0)
    {
      std::cout << "Choose the type of sequence" << std::endl;
      std::cout << "A: String" << std::endl;
      std::cout << "B: Array of integers" << std::endl;
      std::cout << "C: Doubly linked pointer list" << std::endl;
      std::cout << "D: Exit" << std::endl;

    }else if(menuType == 1){
      std::cout << "Choose the type of integer" << std::endl;
      std::cout << "A: 8 bit" << std::endl;
      std::cout << "B: 16 bit" << std::endl;
      std::cout << "C: 32 bit" << std::endl;
      std::cout << "D: 64 bit" << std::endl;
    }

    int choice = 0;

    switch(readChar()){
    case('A'):
    case('a'):
      choice = 0;
      break;
    case('B'):
    case('b'):
      choice = 1;
      break;
    case('C'):
    case('c'):
      choice = 2;
      break;
    case('D'):
    case('d'):
      choice = 3;
      if(menuType == 0)
      {
        exit(0);
      }
      break;
    default:
      std::cout << "Wrong character, try again.";
      continue;
    }
    return choice;
  }
}
