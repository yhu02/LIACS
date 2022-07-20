/* pagetables -- A framework to experiment with memory management
 *
 *    simple/mmu.cc - A simple, single level page table to serve as
 *                    example. Hardware MMU part.
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#include "aarch64.h"
using namespace AArch64;

/*
 * MMU part. The MMU simply performs a walk of the page table and
 * attempts the translation from a virtual to a physical address.
 */

AArch64MMU::AArch64MMU()
{
}

AArch64MMU::~AArch64MMU()
{
}

// multi level (4) page table walk
bool
AArch64MMU::performTranslation(const uint64_t vPage,
                              uint64_t &pPage,
                              bool isWrite)
{
  uint16_t levelEntry[4];

  levelEntry[0] = (vPage & ((1UL << 34) - 1)) >> 33; //L0: bit 33      
  levelEntry[1] = (vPage & ((1UL << 33) - 1)) >> 22; //L1: bits 32-22   
  levelEntry[2] = (vPage & ((1UL << 22) - 1)) >> 11; //L2: bits 21-11
  levelEntry[3] = (vPage & ((1UL << 11) - 1));       //L3: bits 10-0
 
  /* Validate alignment of page table */
  if ((root & (pageTableAlign - 1)) != 0)
    throw std::runtime_error("Unaligned page table access");

  TableEntry *tablePtr = reinterpret_cast<TableEntry *>(root);

  // Perform page table walk
  for(int i = 0; i < 4; i++)
  {
    if (not tablePtr[levelEntry[i]].valid) // Page table entry does not contain a valid page table
      return false;
    
    // enable dirty bit when table entry is a physical page and is being written
    if(i == 3 && isWrite){
      tablePtr[levelEntry[i]].dirtyBitModifier = 1;
    }

    //tablePtr will point to page table entry within previous page table
    tablePtr = reinterpret_cast<TableEntry *>(tablePtr[levelEntry[i]].outputAddr);
  }

  //Address of tablePtr is now the physical page
  pPage = reinterpret_cast<uint64_t>(&tablePtr);

  return true;
}
