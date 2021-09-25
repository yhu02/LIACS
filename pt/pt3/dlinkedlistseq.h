#ifndef _dlinkedlistseq
#define _dlinkedlistseq

#include "standard.h"
#include "dlinkedlistseqvariant.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


template <class T>
class DLinkedListSeq
{
  public:
    DLinkedListSeq();                                              //Constructor
    DLinkedListSeq(int i);                                         //Constructor
    ~DLinkedListSeq();                                             //Destructor
    
    int length() const;                                                  //Amount of symbols in DLinkedListSeq
    char charAt(int const &pos) const;                                          //Char at position pos 
    char getSymbol(int const &number) const;                                    //Takes an integer 0-3 and returns a character ATCG
    void setSeq();                                                 //Reads in sequence and sets value

    DLinkedListSeq<T>* concat(DLinkedListSeq<T>* const &seq2) const;              //Returns concatenation between current sequence and seq2
    DLinkedListSeq<T>* slice(int const &start, int const &end) const;                   //Returns current sequence between start and end
    bool equal(DLinkedListSeq<T> const &seq2) const;                            //Checks if current sequence equals seq2

    void readVariant();                                            //Reads variants from file
    void doVariant();                                              //Applies the variants to the sequence
    void writeResult() const;                                      //Writes current sequence to a file

    void Insertion(Variant* const &var);                           //Inserts a sequence into the current sequence
    void Deletion(Variant* const &var);                            //Deletes part of the current sequence
    void DeletionInsertion(Variant* const &var);                   //Deletes and inserts ^
    void Substitution(Variant* const &var);                        //Special case of deletion and insertion
    void Inversion(Variant* const &var);                           //Inverts a part of the current sequence

  private:
    std::vector<Variant*> variants;                                //Vector with variants read in from a file
    int symbolCount;                                               //Symbol count
    static const int intSize;                                      //Size of an integer in symbols

    class Node                                                     //Node in pointer list
    {
      public:
        Node();
        T number;
        Node* prev;
        Node* next;
    };

    Node* begin;                                                    //Pointer to begin of the pointer list
    Node* end;                                                      //Pointer to end of the pointer list
};  
//Static variables
template <class T>
const int DLinkedListSeq<T>::intSize = sizeof(T) * 4;

//Constructor
template <class T>
DLinkedListSeq<T>::Node::Node()
{
  this->number = 0;
  this->next = this->prev = nullptr;
}

//Constructor
template <class T>
DLinkedListSeq<T>::DLinkedListSeq()
{
  this->symbolCount = 0;
  this->begin = this->end = nullptr;
};

//Constructor for the inital sequence to chain commands
template <class T>
DLinkedListSeq<T>::DLinkedListSeq(int i)
{
  if(i){}; // Get rid of warning
  this->setSeq();
  this->readVariant();
  this->doVariant();
  this->writeResult();
};

//Destructor
template <class T>
DLinkedListSeq<T>::~DLinkedListSeq()
{
  if(this->begin != nullptr)
  {
    Node* iterator = this->begin;
    while(iterator->next != nullptr)
    {
      iterator = iterator->next;
      delete iterator->prev;
    }
    delete iterator;
  }
};

//Returns symbolcount
template <class T>
int DLinkedListSeq<T>::length() const
{
  return this->symbolCount;
};

//Returns char at position pos
template <class T>
char DLinkedListSeq<T>::charAt(int const &pos) const
{
  if(pos >= this->symbolCount)
  {
    std::cout << "Index out of bounds";
    exit(0);
  }

  Node* iterator = this->begin;
  int index = pos / int(sizeof(T) * 4);                                       //Index from pointerlist where the symbol is located
  T number;                                                                   //Number to retrieve symbol from
  int shift = 2 * (int(sizeof(T) * 4) - pos % int(sizeof(T) * 4) - 1);        //Shift amount to get right symbol number to the right 

  for(int i = 0; i < index; i++)
  {
    iterator = iterator->next;
  }

  number = iterator->number;                                          
  number >>= shift;                       //Shift number bits to the right
  number = number & 3;                    //Bitwise & to get a value between 0 and 3                                 

  return this->getSymbol(number);         //Return symbol corresponding to number
};

//Takes integer and returns char
template <class T>
char DLinkedListSeq<T>::getSymbol(int const &number) const
{
  char symbol;
  switch(number)
  {
    case(0):
      symbol = 'A';
      break;
    case(1):
      symbol = 'T';
      break;
    case(2):
      symbol = 'C';
      break;
    case(3):
      symbol = 'G';
      break;
    default:
      break;
  }
  return symbol;
};

//Reads in sequence and sets values
template <class T>
void DLinkedListSeq<T>::setSeq()
{
  std::ifstream input;      
  std::string input_name;
  char symbol;

  do
  {
    std::cout <<"Type the name of the file containing the sequence:\n";
    std::cin >> input_name;
    input.open(input_name,std::ios::in);
    if(input.fail())
    {
      std::cout << "File doesn't exist\n";
    }else{
      break;
    }
  } while(true);
  
  
  while(!input.eof())
  {
    int symbol_number;
    T seqElement = 0;
    Node* newNode = new Node;
    for(int i = 0; i < int(sizeof(T) * 4); i++)        //Bytes * 4 = symbols per type T
    {
      begin:;
      input.get(symbol);
      if(seqElement > 0)
      {
        seqElement <<= 2;
      }
      if(input.eof())
      {
        continue;          //Skip this iteration 
      }
      switch(symbol)
      {
        case('A'):
          symbol_number = 0;
          break;
        case('T'):
          symbol_number = 1;
          break;
        case('C'):
          symbol_number = 2;
          break;
        case('G'):
          symbol_number = 3;
          break;
        default:
          goto begin;                   //Character is invalid, get next character
      }
      seqElement += symbol_number;
      this->symbolCount++;
    }

    newNode->number = seqElement;

    if(this->begin == nullptr)
    {
      this->begin = newNode;
      this->end = newNode;
    }else{
      newNode->prev = this->end;
      this->end->next = newNode;
      this->end = newNode;
    }
  }
 
  if((this->symbolCount % (int(sizeof(T) * 4))) == 0)           //Check if symbol has last been detected in new sequence element
  {
    Node* newNode = this->end;
    this->end = this->end->prev;
    this->end->next = nullptr;
    delete newNode;
  }
  input.close();
};

//Read the variants per line and put them in the vector
template <class T>
void DLinkedListSeq<T>::readVariant()
{
  std::string input_name;
  std::ifstream input;
  
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

  input.open(input_name,std::ios::in);

  //Data containers
  int start;
  int end;
  std::string type;
  std::string seq;
  
  //Stringstream temp variable
  std::string line = "";                  

  while(std::getline(input, line))
  {
    std::stringstream ss(line);
    std::vector<std::string> result;  //Vector with read data

    //Read data from file
    while(ss.good())
    {
      std::string substr;
      getline(ss, substr, ',' );
      result.push_back( substr );
    }

    //Assign saved values
    start = stoi(result[0]);
    end = stoi(result[1]);
    type = result[2];
    seq = "";
    if(result.size() == 4)
    {
      seq = result[3];
    }

    //Put variant into vector
    Variant* newVariant;
    newVariant  = new DLinkedListSeqVariant<T>(start,end,type,seq);
    this->variants.push_back(newVariant);
  } 
  input.close();
};

//Returns concatenation of current sequence and seq2
template <class T>
DLinkedListSeq<T>* DLinkedListSeq<T>::concat(DLinkedListSeq<T>* const &seq2) const
{
  int newSymbolCount = this->symbolCount + seq2->symbolCount;
  int shift = 2 * (int(sizeof(T) * 4) - (this->symbolCount % (sizeof(T) * 4)));  //Shift amount to get the last symbol bits to the right
  T tempNumber = this->end->number;
  T tempNumber2;
  T tempNumber3;
  bool flag = false;

  Node* newBegin = nullptr;
  Node* newEnd = nullptr;

  Node* iterator = this->begin;
  Node* iterator2 = seq2->begin;

  //Copy first pointerlist into new pointerlist
  if(iterator != nullptr)
  {
    do
    {
      Node* newNode = new Node;
      newNode->number = iterator->number;

      if(newBegin == nullptr)
      {
        newBegin = newNode;
        newEnd = newNode;
      }else{
        newNode->prev = newEnd;
        newEnd->next = newNode;
        newEnd = newNode;
      }
      iterator = iterator->next;
    }while(iterator != nullptr);

    if(shift == (2 * (int(sizeof(T) * 4))))       //Dont shift last element if T is fully filled with symbols
    {
      shift = 0;
    }else{
      //Delete the last element
      Node* newNode = newEnd;
      if(newEnd->prev != nullptr)     
      {
        newEnd = newEnd->prev;
        newEnd->next = nullptr;
      }else{                                    
        newEnd = nullptr;
        newBegin = nullptr;
      }
  
      delete newNode;
    }
    
  }

  //Check if seq2 is not empty
  while(iterator2 != nullptr)
  { 
    if(flag == true)
    {
      delete iterator2;
      break;
    }
    
    Node* newNode = new Node;
    //Skip the shifting operations if no shift occurs
    if(shift == 0)
    {
      newNode->number = iterator2->number;
      if(newBegin == nullptr)
      {
        newBegin = newNode;
        newEnd = newNode;
      }else{
        newNode->prev = newEnd;
        newEnd->next = newNode;
        newEnd = newNode;
      }
    }else{
      //Shift
      tempNumber2 = iterator2->number;
      tempNumber3 = iterator2->number;

      tempNumber2 >>= (2 * (sizeof(T) * 4)) - shift;
      tempNumber3 <<= shift;
      tempNumber += tempNumber2;

      newNode->number = tempNumber;
      tempNumber = tempNumber3;

      
      if(newBegin == nullptr)
      {
        newBegin = newNode;
        newEnd = newNode;
      }else{
        newNode->prev = newEnd;
        newEnd->next = newNode;
        newEnd = newNode;
      }
    }
    //Go to next number
    iterator2 = iterator2->next;
    
    //Iterate 1 more time
    if(iterator2 == nullptr || flag == true)
    {
      flag = true;
      iterator2 = new Node;
      iterator2->number = 0;
    }
  }
  //Return new DLinkedListSeq<T> object
  DLinkedListSeq<T>* newArraySeq = new DLinkedListSeq<T>;
  newArraySeq->begin = newBegin;
  newArraySeq->end = newEnd;
  newArraySeq->symbolCount = newSymbolCount;

  return newArraySeq;
};

//Returns current sequence between start and end
template <class T>
DLinkedListSeq<T>* DLinkedListSeq<T>::slice(int const &start, int const &end) const
{
  
  int newSymbolCount = end - start + 1;

  //Bound checking
  if(end > this->symbolCount || start < 0 || start > end)
  {
    std::cout << "Index out of bounds" << std::endl;
    std::cout << start << ":" << end << ":" << this->symbolCount;
    exit(0);
  }

  Node* newBegin = nullptr;
  Node* newEnd = nullptr;
  Node* iterator = nullptr;

  T tempNumber;
  T tempNumber2;

  int startShift = 2 * (start % (sizeof(T) * 4));    //Shift amount to get the start bits to the left
  int endShift = 2 * ((sizeof(T) * 4) - ((end + 1) % (sizeof(T) * 4)));   // Shift amount to get the end bits to the right
  int startIndex  = start / (sizeof(T) * 4);
  int endIndex = end / (sizeof(T) * 4);

  if(endShift == 32)
  {
    endShift = 0;
  }

  iterator = this->begin;

  for(int i = 0; i < startIndex; i++)
  {
    iterator = iterator->next;
  }
  tempNumber = iterator->number;
  
  //Shift nodes
  for(int i = 0; i < (endIndex - startIndex); i++)
  {
    Node* newNode = new Node;
    tempNumber <<= startShift;
    tempNumber2 = iterator->next->number;

    if(i == (endIndex - 1))
    {
      tempNumber2 >>= endShift;
    }else{
      tempNumber2 >>= (2 * (sizeof(T) * 4)) - startShift;
    }

    if(startShift == 0)
    {
      tempNumber2 = 0;
    }
 
    tempNumber2 += tempNumber;
    
    tempNumber = iterator->next->number;


    newNode->number = tempNumber2;
    if(newBegin == nullptr)
    {
      newBegin = newNode;
      newEnd = newNode;
    }else{
      newNode->prev = newEnd;
      newEnd->next = newNode;
      newEnd = newNode;
    }
    iterator = iterator->next;
  }
  //Shift conditions for last node
  if((endIndex == startIndex) || ((startShift + endShift) < 32 ))
  {
    Node* newNode = new Node;

    tempNumber2 = iterator->number;

    tempNumber2 >>= endShift;
    tempNumber2 <<= endShift;
    tempNumber2 <<= startShift;
    tempNumber2 >>= startShift;
    tempNumber2 <<= startShift;

    newNode->number = tempNumber2;
    if(newBegin == nullptr)
    {
      newBegin = newNode;
      newEnd = newNode;
    }else{
      newNode->prev = newEnd;
      newEnd->next = newNode;
      newEnd = newNode;
    }
  }
  
  //Return new DLinkedListSeq<T> object
  DLinkedListSeq<T>* newArraySeq = new DLinkedListSeq<T>;
  newArraySeq->begin = newBegin;
  newArraySeq->end = newEnd;
  newArraySeq->symbolCount = newSymbolCount;

  return newArraySeq;
};

//Checks if current sequence equals seq2
template <class T>
bool DLinkedListSeq<T>::equal(DLinkedListSeq const &seq2) const
{
  Node* iterator = this->begin;
  Node* iterator2 = seq2->begin;

  //Starting conditions
  if(iterator == nullptr && iterator2 == nullptr)
  {
    return true;
  }else if(!(iterator == nullptr) !=  !(iterator2 == nullptr)){   //XOR check if one of the sequences is a nullptr
    return false;
  }
  
  // Iterate through sequences
  while(iterator != nullptr)
  {
    if((iterator->number != iterator2->number) || (iterator2->next == nullptr))
    {
      return false;
    }
    iterator = iterator->next;
    iterator2 = iterator2->next;
  }
  return true;
};

template <class T>
void DLinkedListSeq<T>::doVariant()
{ 
  while(true)
  {
    bool flag = true;

    for(size_t i = 0; i < this->variants.size(); i++)
    {
      if((*variants[i]).type == ">")
      {
        this->Substitution(variants[i]);
      }else if((*variants[i]).type == "ins"){
        this->Insertion(variants[i]);
      }else if((*variants[i]).type == "del"){
        this->Deletion(variants[i]);
      }else if((*variants[i]).type == "delins"){
        this->DeletionInsertion(variants[i]);
      }else if((*variants[i]).type == "inv"){
        this->Inversion(variants[i]);
      } 
    }
    this->writeResult();    
    //Free memory if variants have not been used
    for(size_t i = 0; i < this->variants.size(); i++)
    {
      delete variants[i];
    }  
    this->variants.clear();       //Clear variants

    while(flag)
    {
      Variant *var = new DLinkedListSeqVariant<T>;

      std::cout << "A: Read in variant" << std::endl;
      std::cout << "B: Apply read variants" << std::endl;
      std::cout << "C: Write last read variant to file" << std::endl;
      std::cout << "D: Exit" << std::endl;
      
      switch(readChar()){
      case('A'):
      case('a'):
        var->read();
        std::cin.clear();
        std::cin.ignore();
        this->variants.push_back(var);
        continue;
      case('B'):
      case('b'):
        flag = false;
        break;
      case('C'):
      case('c'):
        if(this->variants.size() >= 1)
        {
          this->variants.back()->write();
        }
        break;
      case('D'):
      case('d'):
        delete var;
        //Free memory if variants have not been used
        for(size_t i = 0; i < this->variants.size(); i++)
        {
          delete variants[i];
        }  
        this->variants.clear();
        return;
      default:
        std::cout << "Invalid character try again" << std::endl;
        break;
      }
      delete var;
    }
  }
};

template <class T>
void DLinkedListSeq<T>::writeResult() const
{
  std::ofstream output;

  output.open ("RESULT.txt",std::ios_base::out);

  std::cout << std::endl << "Resulting sequence" << std::endl;
  for(int i = 0; i < this->length(); i++)
  {
    output << charAt(i);
    std::cout << charAt(i);
  }
  std::cout << std::endl;
  output.close();
};

template<class T>
void DLinkedListSeq<T>::Insertion(Variant* const &var)
{
  if(var){};
};

template<class T>
void DLinkedListSeq<T>::Deletion(Variant* const &var)
{
  if(var){};
};  

template<class T>
void DLinkedListSeq<T>::DeletionInsertion(Variant* const &var)
{
  if(var){};
};       

template<class T>
void DLinkedListSeq<T>::Substitution(Variant* const &var)
{
  if(var){};
};                      

template<class T>
void DLinkedListSeq<T>::Inversion(Variant* const &var)
{
  if(var){};
};
#endif