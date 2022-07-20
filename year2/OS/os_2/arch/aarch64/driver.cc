/* pagetables -- A framework to experiment with memory management
 *
 *    simple/driver.cc - A simple, single level page table to serve as
 *                       example. OS driver part.
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#include "aarch64.h"
using namespace AArch64;

/*
 * Helper functions for generating assertions and forming page table
 * entries and addresses.
 */

static inline void
initPageTableEntry(TableEntry &entry, const uintptr_t address)
{
  entry.outputAddr = address;
  entry.accessPermission = 1;
  entry.valid = 1;
  entry.dirtyBitModifier = 0;
}

static inline uintptr_t
getAddress(TableEntry &entry)
{
  return (uintptr_t)entry.outputAddr << pageBits;
}

/* OSkernel part. The OS kernel is in charge of actually allocating and
 * organizing the page tables.
 */

const static int entries = 1 << (addressSpaceBits - pageBits - 23);

AArch64MMUDriver::AArch64MMUDriver()
    : pageTables(), bytesAllocated(0), kernel(nullptr)
{
}

AArch64MMUDriver::~AArch64MMUDriver()
{
  if (not pageTables.empty())
    std::cerr << "MMUDriver: error: kernel did not release all page tables."
              << std::endl;
}

void AArch64MMUDriver::setHostKernel(OSKernel *kernel)
{
  this->kernel = kernel;
}

uint64_t
AArch64MMUDriver::getPageSize(void) const
{
  return pageSize;
}

void AArch64MMUDriver::allocatePageTable(const uint64_t PID)
{
  TableEntry *table = reinterpret_cast<TableEntry *>(kernel->allocateMemory(entries * sizeof(TableEntry), pageTableAlign));
  bytesAllocated += entries * sizeof(TableEntry);

  for (int i = 0; i < entries; ++i)
  {
    table[i].valid = 0;
    table[i].dirtyBitModifier = 0;
  }

  pageTables.emplace(PID, table);
}

// allocate page table and return as a TableEntry to put into another page table
TableEntry *
AArch64MMUDriver::allocatePageTable2()
{
  TableEntry *table = reinterpret_cast<TableEntry *>(kernel->allocateMemory(entries * sizeof(TableEntry), pageTableAlign));
  bytesAllocated += entries * sizeof(TableEntry);

  for (int i = 0; i < entries; ++i)
  {
    table[i].valid = 0;
    table[i].dirtyBitModifier = 0;
  }
  return table;
}

void AArch64MMUDriver::releasePageTable(const uint64_t PID)
{
  uint16_t entries = (1UL << 11);
  TableEntry *tablePtr0 = reinterpret_cast<TableEntry *>(getPageTable(PID));

  for (int i = 0; i < entries; i++)
  {
    if (tablePtr0[i].valid == 1)
    {
      TableEntry *tablePtr1 = &tablePtr0[i];
      for (int j = 0; j < entries; j++)
      {
        if (tablePtr1[j].valid == 1)
        {
          TableEntry *tablePtr2 = &tablePtr1[j];
          for (int k = 0; k < entries; k++)
          {
            if (tablePtr2[k].valid == 1)
            {
              kernel->releaseMemory(reinterpret_cast<void *>(tablePtr2[k].outputAddr), entries * sizeof(TableEntry));
            }
          }
          kernel->releaseMemory(reinterpret_cast<void *>(tablePtr1[j].outputAddr), entries * sizeof(TableEntry));
        }
      }
      kernel->releaseMemory(reinterpret_cast<void *>(tablePtr0[i].outputAddr), entries * sizeof(TableEntry));
    }
  }

  kernel->releaseMemory(reinterpret_cast<void *>(&tablePtr0), entries * sizeof(TableEntry));
  auto it = pageTables.find(PID);
  pageTables.erase(it);
}

uintptr_t
AArch64MMUDriver::getPageTable(const uint64_t PID)
{
  auto kv = pageTables.find(PID);
  if (kv == pageTables.end())
    return 0x0;
  /* else */
  return reinterpret_cast<uintptr_t>(kv->second);
}

// multi level (4) page table allocation
void AArch64MMUDriver::setMapping(const uint64_t PID,
                                  uintptr_t vAddr,
                                  PhysPage &pPage)
{
  vAddr &= (1UL << addressSpaceBits) - 1;

  TableEntry *tablePtr = pageTables[PID];
  TableEntry *tmp;
  uint16_t levelEntry[4];

  levelEntry[0] = (vAddr & ((1UL << 48) - 1)) >> 47; // L0: bit 47
  levelEntry[1] = (vAddr & ((1UL << 47) - 1)) >> 36; // L1: bits 46-36
  levelEntry[2] = (vAddr & ((1UL << 36) - 1)) >> 25; // L1: bits 35-25
  levelEntry[3] = (vAddr & ((1UL << 25) - 1)) >> 14; // L3: bits 24-14

  for (int i = 0; i < 3; i++)
  {
    tmp = tablePtr;
    if (tablePtr[levelEntry[i]].valid == 0)
    { // Page table entry does not contain a valid page table
      tmp = tablePtr;
      tablePtr = allocatePageTable2();
      initPageTableEntry(tmp[levelEntry[i]], reinterpret_cast<uintptr_t>(tablePtr));
    }
    else
    { // Page table entry contains a valid page table
      tablePtr = reinterpret_cast<TableEntry *>(tablePtr[levelEntry[i]].outputAddr);
    }
  }

  initPageTableEntry(tablePtr[levelEntry[3]], pPage.addr);
  pPage.driverData = &tablePtr[levelEntry[3]];
}

uint64_t
AArch64MMUDriver::getBytesAllocated(void) const
{
  return bytesAllocated;
}

void AArch64MMUDriver::setPageValid(PhysPage &pPage, bool setting)
{
  TableEntry *entry = reinterpret_cast<TableEntry *>(pPage.driverData);
  if (not entry->valid)
    throw std::runtime_error("Access to invalid page table entry attempted.");

  entry->valid = setting;
}
