#ifndef _arrayseqvariant
#define _arrayseqvariant

#include "variant.h"

//Forward declaration
template <class E>
class ArraySeq;

template<class T>
class ArraySeqVariant : public Variant
{ 
    public:
    ArraySeqVariant(int start = 0, int end = 0, std::string type = "", std::string sequence = "");
    ~ArraySeqVariant();

  private:
};

template<class T>
ArraySeqVariant<T>::ArraySeqVariant(int start, int end, std::string type, std::string sequence) : Variant(start, end, type, sequence)
{
  
};

template<class T>
ArraySeqVariant<T>::~ArraySeqVariant()
{

}

#endif