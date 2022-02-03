#ifndef _stringseq
#define _stringseq

#include "standard.h"
#include "variant.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class StringSeq
{
  public:
    StringSeq();                                                  //Default constructor
    StringSeq(std::string const &sequence);                       //Sets sequence to given input string 
    StringSeq(int const &i);                                      //Constructor for the inital sequence to chain commands
    ~StringSeq();                                                 //Default destructor

    int length() const;                                           //Returns symbolcount
    char charAt(int pos) const;                                   //Returns character at position pos
    void setSeq();                                                //Reads sequence from file
    StringSeq* concat(StringSeq* const &seq2) const;              //Returns concatenation of current sequence and seq2
    StringSeq* slice(int const &start, int const &end) const;     //Returns the current sequence starting at pos start and ending at pos end
    bool equal(StringSeq const &seq2) const;                      //Returns true if current sequence equals seq2 else false
    void readVariant();                                           //Reads variants from file
    void doVariant();                                             //Applies the variants to the sequence + menu
    void writeResult() const;                                     //Writes current sequence to a file

    void Insertion(Variant* const &var);                          //Inserts a sequence into the current sequence
    void Deletion(Variant* const &var);                           //Deletes part of the current sequence
    void DeletionInsertion(Variant* const &var);                  //Deletes and inserts
    void Substitution(Variant* const &var);                       //Deletes and inserts
    void Inversion(Variant* const &var);                          //Inverts a part of the current sequence

  private:
    std::vector<Variant*> variants;                               //Vector with variants read in from a file
    std::string sequence;                                         //Sequence
    int symbolCount;                                              //Symbol count in sequence
};

#endif
