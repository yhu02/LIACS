/* pagetables -- A framework to experiment with memory management
 *
 *    simple/driver.cc - A simple, single level page table to serve as
 *                       example. OS driver part.
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#include "simple.h"
using namespace Simple;

#include <iostream>

/*
 * Helper functions for generating assertions and forming page table
 * entries and addresses.
 */

static inline void
initPageTableEntry(TableEntry &entry, const uintptr_t address)
{
  entry.physicalPage = address >> pageBits;
  entry.read = 1;
  entry.valid = 1;
  entry.dirty = 0;
}

static inline uintptr_t
getAddress(TableEntry &entry)
{
  return (uintptr_t)entry.physicalPage << pageBits;
}

/* OSkernel part. The OS kernel is in charge of actually allocating and
 * organizing the page tables.
 */

const static int entries = 1 << (addressSpaceBits - pageBits);

SimpleMMUDriver::SimpleMMUDriver()
  : pageTables(), bytesAllocated(0), kernel(nullptr)
{
}

SimpleMMUDriver::~SimpleMMUDriver()
{
  if (not pageTables.empty())
    std::cerr << "MMUDriver: error: kernel did not release all page tables."
              << std::endl;
}


void
SimpleMMUDriver::setHostKernel(OSKernel *kernel)
{
  this->kernel = kernel;
}

uint64_t
SimpleMMUDriver::getPageSize(void) const
{
  return pageSize;
}

void
SimpleMMUDriver::allocatePageTable(const uint64_t PID)
{
  TableEntry *table = reinterpret_cast<TableEntry *>
      (kernel->allocateMemory(entries * sizeof(TableEntry), pageTableAlign));
  bytesAllocated += entries * sizeof(TableEntry);

  for (int i = 0; i < entries; ++i)
    {
      table[i].valid = 0;
      table[i].dirty = 0;
    }

  pageTables.emplace(PID, table);
}

void
SimpleMMUDriver::releasePageTable(const uint64_t PID)
{
  auto it = pageTables.find(PID);
  kernel->releaseMemory(it->second, entries * sizeof(TableEntry));
  pageTables.erase(it);
}

uintptr_t
SimpleMMUDriver::getPageTable(const uint64_t PID)
{
  auto kv = pageTables.find(PID);
  if (kv == pageTables.end())
    return 0x0;
  /* else */
  return reinterpret_cast<uintptr_t>(kv->second);
}

void
SimpleMMUDriver::setMapping(const uint64_t PID,
                            uintptr_t vAddr,
                            PhysPage &pPage)
{
  /* Strip off the sign-extension in the high 16 bits */
  vAddr &= (1UL << addressSpaceBits) - 1;
  int entry = vAddr / pageSize;

  initPageTableEntry(pageTables[PID][entry], pPage.addr);
  pPage.driverData = &pageTables[PID][entry];
}

uint64_t
SimpleMMUDriver::getBytesAllocated(void) const
{
  return bytesAllocated;
}

void
SimpleMMUDriver::setPageValid(PhysPage &pPage, bool setting)
{
  TableEntry *entry = reinterpret_cast<TableEntry *>(pPage.driverData);
  if (not entry->valid)
    throw std::runtime_error("Access to invalid page table entry attempted.");

  entry->valid = setting;
}
