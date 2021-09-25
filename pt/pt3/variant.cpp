#include "variant.h"
#include <iostream>
Variant::Variant(int start, int end, std::string type, std::string seq)
{
  this->start = start;
  this->end = end;
  this->type = type;
  this->sequence = seq;
};

Variant::~Variant()
{

};

void Variant::read()
{
  bool flagType = false;     //Check if type is correct
  bool flagSymbol = false;     //Check if type is correct
  
  //Input checking
  do
  {
    std::cout << "Enter start position:" << std::endl;
    std::cin >> this->start;
  }while(!std::cin);

  do
  {
    std::cout << "Enter end position:" << std::endl;
    std::cin >> this->end;
  }while(!std::cin);

  do
  {
    std::cout << "Enter type (>,ins,del,delins,inv):" << std::endl;
    std::cin >> this->type;
    if(!((this->type == ">")     ||
        (this->type == "ins")    ||
        (this->type == "del")    ||
        (this->type == "delins") ||
        (this->type == "inv"))){
      flagType = false;                            //Incorrect type has been detected repeat prompt
    }else{
      flagType = true;
    }
  }while(!flagType);

  do
  {
    std::cout << "Enter sequence (ATCG):" << std::endl;
    std::cin >> this->sequence;
    for(size_t i = 0; i < this->sequence.size(); i++)
    {
      if(!((this->sequence[i] == 'A') ||
          (this->sequence[i] == 'T') ||
          (this->sequence[i] == 'C') ||
          (this->sequence[i] == 'G'))){
        flagSymbol = false;                      //Incorrect symbol has been detected repeat prompt
        goto end;
      }   
    }
    break;
    end:;
  }while(!flagSymbol);
};

void Variant::write()
{
  std::ofstream output;

  output.open ("VARIANTS.txt", std::ios::app);
  output << "\n" << this->start << ",";
  output << this->end << ",";
  output << this->type << ",";
  output << this->sequence;
  output.close();
};