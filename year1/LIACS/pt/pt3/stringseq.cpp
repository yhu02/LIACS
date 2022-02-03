#include "stringseq.h"
#include "stringseqvariant.h"

//Constructor
StringSeq::StringSeq()
{
};

StringSeq::StringSeq(std::string const &sequence)
{
  this->sequence = sequence;
  this->symbolCount = sequence.length();
};

//Chain commands
StringSeq::StringSeq(int const &i)
{
  if(i){}; // Get rid of warning
  this->setSeq();
  this->readVariant();
  std::cin.clear();
  std::cin.ignore();
  this->doVariant();
};

//Destructor
StringSeq::~StringSeq()
{
  for(size_t i = 0; i < this->variants.size(); i++)
  {
    delete this->variants[i];
  }
};

//Returns symbolcount
int StringSeq::length() const
{
  return this->symbolCount;
};

//Returns character at position pos
char StringSeq::charAt(int pos) const
{
  return this->sequence[pos];
};

//Reads sequence from file 
void StringSeq::setSeq()
{
  std::ifstream input;      
  std::string input_name;
  char symbol = 0;

  do
  {
    std::cout <<"Type the name of the file containing the sequence:\n";
    std::cin >> input_name;
    input.open(input_name,std::ios::in);
    if(input.fail())
    {
      std::cout << "File doesn't exist\n";
      input_name = "";
    }else{
      break;
    }
  } while(true);
    
  while(input.get(symbol))
  {
    this->sequence = sequence + symbol;
  }
  input.close();
  this->symbolCount = sequence.length();
};

//Returns concatenation of current sequence and seq2
StringSeq* StringSeq::concat(StringSeq* const &seq2) const
{
  int newSymbolCount = this->symbolCount + seq2->symbolCount;
  StringSeq* newSeq = new StringSeq;

  newSeq->sequence = this->sequence;
  newSeq->sequence.append(seq2->sequence);
  newSeq->symbolCount = newSymbolCount;
  
  return newSeq;
};

//Returns the current sequence starting at pos start and ending at pos end
StringSeq* StringSeq::slice(int const &start, int const &end) const
{
  //Bound checking
  if(end > this->symbolCount || start < 0 || start > end)
  {
    std::cout << "Index out of bounds" << std::endl;
    std::cout << end << ":" << this->symbolCount << ":" << start << ":" << end << std::endl;
    exit(0);
  }

  int newSymbolCount = end - start;
  StringSeq* newSeq = new StringSeq;

  newSeq->sequence = this->sequence.substr(start, end - start);
  newSeq->symbolCount = newSymbolCount;

  return newSeq;
};

//Returns true if current sequence equals seq2 else false
bool StringSeq::equal(StringSeq const &seq2) const
{
  bool val = this->sequence.compare(seq2.sequence);
  if(val == 0)
  {
    return true;
  }
  return false;
};


 //Reads variants from file
void StringSeq::readVariant()
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

  //Read variants line by line
  while(std::getline(input, line))
  {
    std::stringstream ss(line);
    std::vector<std::string> result;    //Vector with read data

    //Read data from file
    while(ss.good())
    {
      std::string substr;
      getline( ss, substr, ',' );
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
    StringSeqVariant* newVariant = new StringSeqVariant(start,end,type,seq);
    this->variants.push_back(newVariant);
  } 
  input.close();
};

//Applies the variants to the sequence
void StringSeq::doVariant()
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
      Variant *var = new StringSeqVariant;

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

//Writes current sequence to a file
void StringSeq::writeResult() const
{
  std::ofstream output;
  output.open ("RESULT.txt", std::ios_base::out);
  std::cout << std::endl << "Resulting sequence" << std::endl;
  output << this->sequence;
  std::cout << this->sequence << std::endl;
  output.close();
};

//Inserts a sequence into the current sequence
void StringSeq::Insertion(Variant* const &var)
{
  StringSeq *newSeqStart, *newSeqMid, *newSeqEnd, *newSeq4, *newSeq5;

  //Start of sequence
  newSeqStart = this->slice(0, var->start);

  //Inserted sequence
  newSeqMid = new StringSeq(var->sequence);

  //End of sequence
  newSeqEnd = this->slice(var->start, this->length());
  
  //Concatenate sequences
  newSeq4 = newSeqStart->concat(newSeqMid);
  newSeq5 = newSeq4->concat(newSeqEnd);
  
  this->sequence = newSeq5->sequence;
  this->symbolCount = this->length() + sequence.length();

  delete newSeqStart;
  delete newSeqMid;
  delete newSeqEnd;
  delete newSeq4;
  delete newSeq5;
};

//Deletes part of the current sequence
void StringSeq::Deletion(Variant* const &var)
{
  StringSeq *newSeqStart, *newSeqEnd, *newSeq3;
  
  //Start of sequence
  newSeqStart = this->slice(0, var->start);

  //End of sequence
  newSeqEnd = this->slice(var->end, this->length());

  //Concatenate sequences
  newSeq3 = newSeqStart->concat(newSeqEnd);
  
  this->sequence = newSeq3->sequence;
  this->symbolCount = this->length() - (var->end - var->start);

  delete newSeqStart;
  delete newSeqEnd;
  delete newSeq3;
};

//Deletes and inserts
void StringSeq::DeletionInsertion(Variant* const &var)
{
  this->Deletion(var);
  this->Insertion(var);
};

//Deletes and inserts
void StringSeq::Substitution(Variant* const &var)
{
  this->DeletionInsertion(var);
};

//Inverts a part of the current sequence
void StringSeq::Inversion(Variant* const &var)
{

  StringSeq *newSeqStart, *newSeqEnd, *newSeq3, *newSeq4;
  StringSeq *reverseSeq = new StringSeq;


  //Start of sequence
  newSeqStart = this->slice(0, var->start);

  //Build reverse sequence
  for(int i = 0; i < (var->end - var->start); i++)
  {
    StringSeq *temp = this->slice(var->end - 1 - i , var->end - i);
    StringSeq *temp2 = reverseSeq->concat(temp);

    reverseSeq = temp2;

    delete temp;
    delete temp2;
  }

  //End of sequence
  newSeqEnd = this->slice(var->end, this->length());

  //Concatenate sequences
  newSeq3 = newSeqStart->concat(reverseSeq);
  newSeq4 = newSeq3->concat(newSeqEnd);
  
  this->sequence = newSeq4->sequence;

  delete newSeqStart;
  delete newSeqEnd;
  delete reverseSeq;
  delete newSeq3;
  delete newSeq4;
};
