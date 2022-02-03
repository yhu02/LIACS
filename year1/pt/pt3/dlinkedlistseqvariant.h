#ifndef _dlinkedlistseqvariant
#define _dlinkedlistseqvariant

#include "variant.h"

//Forward declaration
template <class E>
class DLinkedListSeq;

template<class T>
class DLinkedListSeqVariant : public Variant
{
  public:
    DLinkedListSeqVariant(int start = 0, int end = 0, std::string type = "", std::string sequence = "");
    ~DLinkedListSeqVariant();

};

template<class T>
DLinkedListSeqVariant<T>::DLinkedListSeqVariant(int start, int end, std::string type, std::string sequence) : Variant(start, end, type, sequence)
{
  
};

template<class T>
DLinkedListSeqVariant<T>::~DLinkedListSeqVariant()
{
  
}


#endif