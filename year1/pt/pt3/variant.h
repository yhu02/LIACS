#ifndef _variant
#define _variant

#include <string>
#include <iostream>
#include <fstream>

class Variant
{
  public:
    Variant(int start = 0, int end = 0, std::string type = "", std::string seq = "");
    virtual ~Variant() = 0;
    void read();
    void write();
    
    int start;
    int end;
    std::string type;
    std::string sequence;
};

#endif