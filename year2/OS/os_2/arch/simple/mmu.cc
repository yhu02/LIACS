/* pagetables -- A framework to experiment with memory management
 *
 *    simple/mmu.cc - A simple, single level page table to serve as
 *                    example. Hardware MMU part.
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#include "simple.h"
using namespace Simple;

/*
 * MMU part. The MMU simply performs a walk of the page table and
 * attempts the translation from a virtual to a physical address.
 */

SimpleMMU::SimpleMMU()
{
}

SimpleMMU::~SimpleMMU()
{
}

bool
SimpleMMU::performTranslation(const uint64_t vPage,
                              uint64_t &pPage,
                              bool isWrite)
{
  /* Strip off the sign-extension in the high 16 bits */
  const int entry = vPage & ((1UL << (addressSpaceBits - pageBits)) - 1);

  /* Validate alignment of page table */
  if ((root & (pageTableAlign - 1)) != 0)
    throw std::runtime_error("Unaligned page table access");

  TableEntry *table = reinterpret_cast<TableEntry *>(root);
  if (not table[entry].valid)
    return false;

  pPage = table[entry].physicalPage;

  return true;
}
