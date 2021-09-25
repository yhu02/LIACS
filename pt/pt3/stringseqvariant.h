#ifndef _stringseqvariant
#define _stringseqvariant

#include "variant.h"
#include "stringseq.h"
#include <string>

class StringSeqVariant : public Variant
{
  public:
    StringSeqVariant(int start = 0, int end = 0, std::string type = "", std::string sequence = "");
    ~StringSeqVariant();
  private:
  
    friend class StringSeq;
};

#endif