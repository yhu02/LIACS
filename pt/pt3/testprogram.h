
#ifndef _testprogram
#define _testprogram

#include "stringseq.h"
#include "arrayseq.h"
#include "dlinkedlistseq.h"
#include "standard.h"
#include <iostream>

class TestProgram
{
  public:
    TestProgram();          //Constructor
    ~TestProgram();         //Destructor
    std::string read();     //Checks if a file can be read and returns the input name
    int menu(int menuType); //Menu to choose integer type and data type and returns choice
  private:
};

#endif