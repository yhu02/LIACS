#ifndef _arrayseq
#define _arrayseq

#include "arrayseqvariant.h"
#include "standard.h"
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

template <class T>
class ArraySeq
{
  public:
    ArraySeq();                                                    //Default constructor
    ArraySeq(int i);                                               //Constructor for the inital sequence to chain commands
    ArraySeq(std::string sequence);                                //Sets sequence to given input string 
    ~ArraySeq();                                                   //Default destructor

    int length() const;                                            //Returns amount of symbols in sequence
    char charAt(int const &pos) const;                             //Returns char at position pos in symbols
    int setSize(std::string const &input_name);                    //Pre reads file to calculate needed memory for array, sets symbolcount and returns size
    char getSymbol(int const &number) const;                       //Takes an integer 0-3 and returns a character ATCG
    void setSeq(std::string input_name = "");                      //Reads in file with sequence and sets value

    ArraySeq<T>* concat(ArraySeq<T>* const &seq2) const;           //Returns concatenation of current sequence and seq2
    ArraySeq<T>* slice(int const &start, int const &end) const;    //Returns sequence between position start and end of the current sequence
    bool equal(ArraySeq const &seq2) const;                        //Checks whether the current sequence and seq2 are equal

    void readVariant();                                            //Reads variants from file
    void doVariant();                                              //Applies the variants to the sequence + menu
    void writeResult() const;                                      //Writes current sequence to a file

    void Insertion(Variant* const &var);                           //Inserts a sequence into the current sequence
    void Deletion(Variant* const &var);                            //Deletes part of the current sequence
    void DeletionInsertion(Variant* const &var);                   //Deletes and inserts ^
    void Substitution(Variant* const &var);                        //Special case of deletion and insertion
    void Inversion(Variant* const &var);                           //Inverts a part of the current sequence
                            
  private:
    std::vector<Variant*> variants;                                //Vector with variants read in from a file
    int symbolCount;                                               //Symbol count in sequence
    T* sequence;                                                   //Pointer to the start of the sequence
    static const int intSize;                                      //Size of an integer in symbols
};

//Static variables
template <class T>
const int ArraySeq<T>::intSize = sizeof(T) * 4;

//Constructor
template <class T>
ArraySeq<T>::ArraySeq()
{
  this->symbolCount = 0;
  this->sequence = nullptr;
};

//Constructor for the inital sequence to chain commands
template <class T>
ArraySeq<T>::ArraySeq(int i)
{
  if(i){}; // Get rid of warning
  this->setSeq();
  this->readVariant();
  std::cin.clear();
  std::cin.ignore();
  this->doVariant();
};

template <class T>
ArraySeq<T>::ArraySeq(std::string sequence)
{
  std::ofstream output;
  output.open ("TEMP.txt",std::ios::out);
 
  output << sequence;
  
  output.close();
  this->setSeq("TEMP.txt");
};

//Destructor
template <class T>
ArraySeq<T>::~ArraySeq()
{
  
  if(this->sequence != nullptr)
  {
    delete[] this->sequence;
  }
};

//Returns symbolcount
template <class T>
int ArraySeq<T>::length() const
{
  return this->symbolCount;
};

template <class T>
char ArraySeq<T>::charAt(int const &pos) const
{
  int index = pos / intSize;                                       //Index from array T where the symbol is located
  T number = this->sequence[index];                                           //Number to retrieve symbol from
  int shift = 2 * (intSize - (pos % intSize) - 1);        //Shift amount to get right symbol number to the right 

  if(pos >= this->symbolCount)
  {
    std::cout << "Index out of bounds";
    exit(0);
  }

  number >>= shift;                       //Shift number bits to the right
  number = number & 3;                    //Bitwise & to get a value between 0 and 3                                 

  return this->getSymbol(number);         //Return symbol corresponding to number
};


template <class T>
int ArraySeq<T>::setSize(std::string const &input_name)
{
  //Initialisation
  int count = 0;
  std::ifstream input;
  char symbol;
  this->symbolCount = 0;
  input.open(input_name,std::ios::in);

  while(!(input.eof()))
  {
    bool flag = false; //Flag to check if symbol has been placed in integer

    //Pack intSize symbols in an integer
    for(int i = 0; i < intSize; i++) 
    {
      begin:;
      input.get(symbol);
      if(input.eof())
      {
        continue;                        //Skip this interation
      }
      switch(symbol)
      {
        case('A'):
        case('T'):
        case('C'):
        case('G'):
          flag = true;                  //Check if symbol has been placed
          this->symbolCount++;          //Keep track of symbolCount
          break;
        default:
          goto begin;                   //Character is invalid, get next character
      }
    }
    //Check if symbol has been detected in new integer
    if(flag == true)
    {
      count++;
    }
  }
  input.close();

  return count;                          //Return size of sequence in integers
}

//Takes integer and returns character
template <class T>
char ArraySeq<T>::getSymbol(int const &number) const
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
}

//Reads file and sets sequence 
template <class T>
void ArraySeq<T>::setSeq(std::string input_name)
{
  std::ifstream input;      
  int size;
  char symbol;
  int count = 0;

  do
  {
    if(input_name == "")
    {
      std::cout <<"Type the name of the file containing the sequence:\n";
      std::cin >> input_name;
    }
    input.open(input_name,std::ios::in);
    if(input.fail())
    {
      std::cout << "File doesn't exist\n";
      input_name = "";
    }else{
      input.close();
      size = this->setSize(input_name);               //Pre read file to know how much memory to allocate
      input.open(input_name);
      break;
    }
  } while(true);

  this->sequence = new T[size]();                        //Allocate memory
    
  while(!input.eof())
  {
    bool flag = false;  //Flag to check if symbol has been placed in integer
    int symbol_number;
    T seqElement = 0;

    //Pack intSize symbols in an integer
    for(int i = 0; i < intSize; i++)  
    {
      begin:;      
      input.get(symbol);

      //Shift elements to the left after every iteration
      if(seqElement > 0)
      {
        seqElement <<= 2;
      }

      if(input.eof())
      {
        continue;       //Skip if eof has been reached
      }
      //Convert symbol to binary value
      switch(symbol)
      {
        case('A'):
          symbol_number = 0;
          flag = true;
          break;
        case('T'):
          symbol_number = 1;
          flag = true;
          break;
        case('C'):
          symbol_number = 2;
          flag = true;
          break;
        case('G'):
          symbol_number = 3;
          flag = true;
          break;
        default:
          goto begin;                   //Character is invalid, get next character
      }
      seqElement += symbol_number;
    }
    //Check if symbol has been detected in element
    if(flag)
    {
      this->sequence[count] = seqElement;
      count++;
    }
  }
  input.close();
};


//Returns concatenation of current sequence and seq2
template <class T>
ArraySeq<T>* ArraySeq<T>::concat(ArraySeq<T>* const &seq2) const
{
  int seqSize = std::ceil(this->symbolCount / double(intSize));                    //Size of current sequence in integers
  int seq2Size = std::ceil(seq2->symbolCount / double(intSize));                   //Size of seq2 sequence in integers
  int newSymbolCount = this->symbolCount + seq2->symbolCount;                      //New symbolcount after concatenation
  int newSize = std::ceil(newSymbolCount / double(intSize));                       //New size in integers after concatenation
  int shift = (2 * (intSize - (this->symbolCount % intSize))) %  (2 * (intSize));  //Shift amount to move the last symbol bits to the right
  int endShift  = ((2 * intSize) - shift) % (2 * (intSize));                       //Shift amount to move the last symbol n bits to the left

  T tempNumber = (seqSize >= 1) ? this->sequence[seqSize - 1] : 0;                 //Value of last integer of current sequence or 0 if size is 0
  T tempNumber2;                                                                   

  T* newSeq = nullptr;

  if(newSize != 0)
  {
    newSeq = new T[newSize]();
    //Copy first sequence into new array
    for(int i = 0; i < seqSize; i++)
    {
      newSeq[i] = this->sequence[i];
    }

    //If shift is 0, then just copy no shifting needed
    if(shift == 0)
    {
      for(int i = 0; i < seq2Size; i++)
      {
        newSeq[i+seqSize] = seq2->sequence[i];
      }
    }else{  
      
      //Iterate over seq2 sequence
      for(int i = 0; i < seq2Size; i++)
      {
        tempNumber2 = seq2->sequence[i];                   //Current integer
        tempNumber2 >>= endShift;                          //Shift integer to the right to merge with previous integer
      
        tempNumber += tempNumber2;                         //Add value of previous integer to current integer
        newSeq[i+seqSize-1] = tempNumber;                  //Assign value

        tempNumber =  seq2->sequence[i];                   //Remember value of current integer
        tempNumber <<= shift;                              //Shift bits to the left to later merge with next integer
      }
    }
  }

  //Returns ArraySeq object with the new sequence
  ArraySeq<T>* newArraySeq = new ArraySeq<T>;
  newArraySeq->sequence = newSeq;
  newArraySeq->symbolCount = newSymbolCount;

  return newArraySeq;
};



//Returns current sequence between start and end
template <class T>
ArraySeq<T>* ArraySeq<T>::slice(int const &start, int const &end) const
{
  //Bound checking
  if(end > this->symbolCount || start < 0 || start > end)
  {
    std::cout << "Index out of bounds" << std::endl;
    exit(0);
  }

  int newSymbolCount = end - start;
  int newSize = std::ceil(newSymbolCount / double(intSize));

  int startShift = (2 * (start % intSize)) % (2 * intSize);            //Shift amount to get the start bits to the left
  int endShift = (2 * (intSize - (end % intSize))) % (2 * intSize) ;   // Shift amount to get the end bits to the right
  int startIndex  = start / intSize;
  int endIndex = (end-1) / intSize;

  T* newSeq = nullptr;

  T tempNumber;                                 //Current integer
  T tempNumber2;                                //Next integer

  if(newSize >= 1)
  {
    newSeq = new T[newSize]();
    //Loop through subarray
    for(int i = startIndex; i < (endIndex + 1); i++)
    {
      if(newSize >= 1)
      {
        if(i < endIndex)
        {
          tempNumber = this->sequence[i];         //Current integer
          tempNumber2 = this->sequence[i + 1];    //Next integer

          tempNumber <<= startShift;              //Shift current integer to the left
          //If no shift occurs, then next integer doesnt get shifted into first integer
          if(startShift == 0)                             
          {
            tempNumber2 = 0;
          }else{
            //Shift next integer to the right to later merge with current integer
            tempNumber2 >>= (2 * intSize) - startShift;
          }
        //Do for last integer in sequence
        }else{
          tempNumber2 = 0;                        //There is no next integer to shift into the last integer
          tempNumber = this->sequence[endIndex];  //Last integer

          //Shift last integer to fit with previous integer
          tempNumber >>= endShift;                
          tempNumber <<= endShift;
          tempNumber <<= startShift;
        }
        //Add current and next integer
        tempNumber += tempNumber2;
        newSeq[i - startIndex] = tempNumber;
      }
    }
  }

  //Return new ArraySeq object with new sequence
  ArraySeq<T>* newArraySeq = new ArraySeq<T>;
  newArraySeq->sequence = newSeq;
  newArraySeq->symbolCount = newSymbolCount;

  return newArraySeq;
};


//Checks if current sequence equals seq2
template <class T>
bool ArraySeq<T>::equal(ArraySeq<T> const &seq2) const
{
  for(int i = 0; i < this->length(); i++)
  {
    if(this->sequence[i] != seq2.sequence[i])
    {
      return false;
    }
  }
  return true;
};

//Read the variants per line and put them in the vector
template <class T>
void ArraySeq<T>::readVariant()
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
    newVariant  = new ArraySeqVariant<T>(start,end,type,seq);
    this->variants.push_back(newVariant);
  } 
  input.close();
};

template<class T>
void ArraySeq<T>::doVariant()
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
      Variant *var = new ArraySeqVariant<T>;

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

template<class T>
void ArraySeq<T>::writeResult() const
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
void ArraySeq<T>::Insertion(Variant* const &var)
{

  ArraySeq<T> *newSeqStart, *newSeqMid, *newSeqEnd, *newSeq4, *newSeq5;

  //Start of sequence
  newSeqStart = this->slice(0, var->start);

  //Inserted sequence
  newSeqMid = new ArraySeq<T>(var->sequence);

  //End of sequence
  newSeqEnd = this->slice(var->start, this->length());

  //Concatenate sequences
  newSeq4 = newSeqStart->concat(newSeqMid);
  newSeq5 = newSeq4->concat(newSeqEnd);

  delete[] this->sequence; 
  
  this->sequence = newSeq5->sequence;
  this->symbolCount = this->length() + var->sequence.length();

  delete[] newSeqStart->sequence;
  delete[] newSeqMid->sequence;
  delete[] newSeqEnd->sequence;
  delete[] newSeq4->sequence;

  newSeqStart->sequence = nullptr;
  newSeqMid->sequence = nullptr;
  newSeqEnd->sequence = nullptr;
  newSeq4->sequence = nullptr;
  newSeq5->sequence = nullptr;

  delete newSeqStart;
  delete newSeqMid;
  delete newSeqEnd;
  delete newSeq4;
  delete newSeq5;
}

template<class T>
void ArraySeq<T>::Deletion(Variant* const &var)
{
  ArraySeq<T> *newSeqStart, *newSeqEnd, *newSeq3;

  //Starting of sequence

  newSeqStart = this->slice(0, var->start);

  //End of sequence
  newSeqEnd = this->slice(var->end, this->length());

  //Concatenate sequences
  newSeq3 = newSeqStart->concat(newSeqEnd);
  
  delete[] this->sequence;

  this->sequence = newSeq3->sequence;
  this->symbolCount = this->length() - (var->end - var->start);

  delete[] newSeqStart->sequence;
  delete[] newSeqEnd->sequence;

  newSeqEnd->sequence = nullptr;
  newSeqEnd->sequence = nullptr;
  newSeq3->sequence = nullptr;

  delete newSeqStart;
  delete newSeqEnd;
  delete newSeq3;
};

template<class T>
void ArraySeq<T>::DeletionInsertion(Variant* const &var)
{
  this->Deletion(var);
  this->Insertion(var);
};

template<class T>
void ArraySeq<T>::Substitution(Variant* const &var)
{
  this->DeletionInsertion(var);
};

template<class T>
void ArraySeq<T>::Inversion(Variant* const &var)
{
  ArraySeq<T> *newSeqStart, *newSeqEnd, *newSeq3, *newSeq4;
  ArraySeq<T> *reverseSeq = new ArraySeq<T>;

  //Start of sequence
  newSeqStart = this->slice(0, var->start);

  //Build reverse sequence
  for(int i = 0; i < (var->end - var->start); i++)
  {
    ArraySeq<T> *temp = this->slice(var->end - 1 - i , var->end - i);
    ArraySeq<T> *temp2 = reverseSeq->concat(temp);

    if(reverseSeq->sequence != nullptr)     //Initial pointer is to nullptr
    {
      delete[] reverseSeq->sequence;        //Delete allocated memory
    }

    reverseSeq->symbolCount++;
    reverseSeq->sequence = temp2->sequence;

    delete[] temp->sequence;

    temp->sequence = nullptr;
    temp2->sequence = nullptr;

    delete temp;
    delete temp2;
  }

  //End of sequence
  newSeqEnd = this->slice(var->end, this->length());

  newSeq3 = newSeqStart->concat(reverseSeq);
  newSeq4 = newSeq3->concat(newSeqEnd);

  delete[] this->sequence;

  this->sequence = newSeq4->sequence;

  delete[] newSeqStart->sequence;
  delete[] reverseSeq->sequence;
  delete[] newSeqEnd->sequence;
  delete[] newSeq3->sequence;

  newSeqStart->sequence = nullptr;
  reverseSeq->sequence = nullptr;
  newSeqEnd->sequence = nullptr;
  newSeq3->sequence = nullptr;
  newSeq4->sequence = nullptr;
  
  delete newSeqStart;
  delete reverseSeq;
  delete newSeqEnd;
  delete newSeq3;
  delete newSeq4;
};
#endif