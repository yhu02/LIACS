/* pagetables -- A framework to experiment with memory management
 *
 *    mmu.cc - Memory Management Unit component
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#include "mmu.h"
#include "settings.h"

#include <iostream>

void MMU::TLB_LRU::flush()
{
  nFlushEvictions += dq.size();
  nFlush++;
}

// TLB which contain entries with vPage, pPage, and ASID
void MMU::TLB_LRU::insert(std::pair<uint64_t, std::pair<uint64_t, uint8_t>> x)
{
  // not present in TLB
  if (ma.find(x.first) == ma.end())
  {
    // TLB is full
    if (dq.size() == size)
    {
      // delete least recently used element
      uint64_t last = dq.back().first;

      // Pops the last element
      dq.pop_back();

      // Erase the last
      ma.erase(last);

      nEvictions++;
    }
  }

  // present in TLB
  else
  {
    dq.erase(ma[x.first]);
  }

  // update reference
  dq.push_front(x);
  ma[x.first] = dq.begin();
}

// Find a given vPage and ASID within the TLB
uint64_t MMU::TLB_LRU::find(const uint8_t &asid, const uint64_t &vPage, bool &valid)
{
  nLookups++;
  // Return table entry where vPage matches the one in the TLB and ASID matches with the one in the page table entry
  auto it = std::find_if(dq.begin(), dq.end(),
                         [&vPage, &asid](const std::pair<uint64_t, std::pair<uint64_t, uint8_t>> &entry)
                         {
                           return (entry.first == vPage && entry.second.second == asid);
                         });

  // Check if physical page has been found
  if (it != dq.end())
  {
    nHits++;
    valid = 1;
    return (*it).second.first;
  }
  else
  {
    valid = 0;
    return 0;
  }
}

// move data to array for use in a different function
int *MMU::TLB_LRU::moveStats()
{
  int *arr = new int[5];

  arr[0] = nLookups;
  arr[1] = nHits;
  arr[2] = nEvictions;
  arr[3] = nFlush;
  arr[4] = nFlushEvictions;

  return arr;
};

MMU::MMU()
    : root(0x0), pageFaultHandler(), tlb(TLBEntries), asidTable{}
{
}

MMU::~MMU()
{
  int nLookups{}, nHits{}, nEvictions{}, nFlush{}, nFlushEvictions{};
  getTLBStatistics(nLookups, nHits, nEvictions, nFlush, nFlushEvictions);

  std::cerr << std::dec << std::endl
            << "TLB Statistics (since last reset):" << std::endl
            << "# lookups: " << nLookups << std::endl
            << "# hits: " << nHits
            << " (" << (((float)nHits / nLookups) * 100.) << "%)" << std::endl
            << "# line evictions: " << nEvictions << std::endl
            << "# flushes: " << nFlush << std::endl
            << "# line evictions due to flush: " << nFlushEvictions << std::endl;
}

void MMU::initialize(PageFaultFunction _pageFaultHandler)
{
  pageFaultHandler = _pageFaultHandler;
}

void MMU::setPageTablePointer(const uintptr_t _root)
{
  root = _root;
}

void MMU::processMemAccess(const MemAccess &access)
{
  if (root == 0x0)
    throw std::runtime_error("MMU: page table pointer is NULL, cannot continue.");

  if (LogMemoryAccesses)
    std::cerr << "MMU: memory access: " << access << std::endl;

  bool valid;
  uint8_t asid;
  uint64_t pAddr;
  uint64_t vPage = access.addr >> getPageBits();

  pAddr = tlb.find(root, vPage, valid);

  if (!valid)
  {
    while (not getTranslation(access, pAddr))
    {
      pageFaultHandler(access.addr);
    }
  }

  if (UseASID)
  {
    // We will use the root pointer as the index for the ASID which is substituting for PID
    if (asidTable.find(root) == asidTable.end())
    {
      asidTable.insert(std::pair<uint64_t, uint8_t>(root, asidTable.size()));
    }
    asid = asidTable.at(root);
  }else{
    // ASID is not enabled so we will just use a constant ASID for all processes
    asid = 0;
  }
  std::pair<uint64_t, uint8_t> TLBEntry(pAddr, asid);
  tlb.insert(std::pair<uint64_t, std::pair<uint64_t, uint8_t>>(vPage, TLBEntry));

  if (LogMemoryAccesses)
    std::cerr << "MMU: translated virtual "
              << std::hex << std::showbase << access.addr
              << " to physical " << pAddr << std::endl;
}

uint64_t
MMU::makePhysicalAddr(const MemAccess &access, const uint64_t pPage)
{
  uint64_t pAddr = pPage << getPageBits();
  pAddr |= access.addr & (getPageSize() - 1);

  return pAddr;
}

bool MMU::getTranslation(const MemAccess &access, uint64_t &pAddr)
{
  const uint64_t vPage = access.addr >> getPageBits();
  uint64_t pPage = 0;
  bool isWrite = (access.type == MemAccessType::Store ||
                  access.type == MemAccessType::Modify);

  if (performTranslation(vPage, pPage, isWrite))
  {
    pAddr = makePhysicalAddr(access, pPage);
    return true;
  }

  return false;
}

void MMU::getTLBStatistics(int &nLookups, int &nHits, int &nEvictions,
                           int &nFlush, int &nFlushEvictions)
{
  int *arr = tlb.moveStats();

  nLookups = arr[0];
  nHits = arr[1];
  nEvictions = arr[2];
  nFlush = arr[3];
  nFlushEvictions = arr[4];

  delete arr;
}
