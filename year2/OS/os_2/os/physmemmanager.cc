/* pagetables -- A framework to experiment with memory management
 *
 *    physmemmanager.h - Physical Memory Manager
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#include "physmemmanager.h"

#include <vector>
#include <iostream>
#include <string.h>
#include <sys/mman.h> /* mmap() */

PhysMemManager::PhysMemManager(const uint64_t pageSize,
                               const uint64_t memorySize)
    : baseAddress(nullptr), pageSize(pageSize), memorySize(memorySize),
      nPages(memorySize / pageSize), nAllocatedPages(0), maxAllocatedPages(0),
      allocated(nPages, false), holeList{}
{
  /* Circuit breaker: avoid too large memory allocations to avoid the user
   * of this program getting into trouble. We limit at 2 GiB.
   */
  if (memorySize > 2ULL * 1024 * 1024 * 1024)
    throw std::runtime_error("automatic protection: attempted to allocate more than 2 GiB of memory.");

  /* Try to allocate "physical" memory. */
  baseAddress = mmap((void *)physMemBase, memorySize,
                     PROT_READ | PROT_WRITE,
                     MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (baseAddress == MAP_FAILED)
    throw std::runtime_error("mmap for physical memory failed: " + std::string(strerror(errno)));

  // Initialise holeList with maximum pages and base address
  std::pair<uint64_t, uint64_t> entry(nPages, reinterpret_cast<uint64_t>(baseAddress));
  holeList.push_back(entry);

  std::cerr << "BOOT: system memory @ "
            << std::hex << std::showbase << baseAddress
            << " page size of " << pageSize << " bytes, "
            << std::dec
            << (memorySize / pageSize) << " pages available." << std::endl;
}

PhysMemManager::~PhysMemManager()
{
  munmap(baseAddress, memorySize);
}

//Hole list based physical memory manager
bool PhysMemManager::allocatePages(size_t count, uintptr_t &addr)
{
  /* Check upfront if pages are available at all. */
  if (nAllocatedPages + count > nPages)
    return false;

  // Iterate through every hole and check if there is space
  for (size_t i = 0; i < holeList.size(); i++)
  {
    //Pages left after using the first count pages from holeList[i].first
    int64_t leftOverPages = holeList[i].first - count;
    if (leftOverPages >= 0)
    {
      
      addr = holeList[i].second;
      nAllocatedPages += count;
      maxAllocatedPages = std::max(maxAllocatedPages, nAllocatedPages);

      if (leftOverPages > 0)
      {
        std::pair<uint64_t, uint64_t> newHole(leftOverPages, holeList[i].second + count * pageSize);
        // Insert smaller hole
        holeList.insert(holeList.begin() + i, newHole);
        // Delete the old hole
        holeList.erase(holeList.begin() + i + 1);
      }else{
        //Hole is completely filled, no smaller hole to replace it
        holeList.erase(holeList.begin() + i);
      }
      return true;
    }
  }

  return false;
}

void PhysMemManager::releasePages(uintptr_t addr, size_t count)
{
  //Check not within mapped memory region then error
  if ((addr < reinterpret_cast<uint64_t>(baseAddress)) ||
     ((addr + count * pageSize) >= (reinterpret_cast<uint64_t>(baseAddress) + nPages * pageSize)))
  {
    return; // error
  }
  else
  {
    nAllocatedPages -= count;
    if (holeList.size() == 0)
    {
      std::pair<uint64_t, uint64_t> newHole(count, (uint64_t)addr);
      holeList.push_back(newHole);
    }
    else
    {
      std::pair<uint64_t, uint64_t> newHole;
      int i = 0;
      while(holeList[i].first)
      {
        //A hole has been added in a previous iteration, no more merging can be done
        if(newHole.first != 0)
          break;

        //Replace hole with larger hole
        if((holeList[i].second + (holeList[i].first + 1) * pageSize) ==  addr)
        {
          newHole = std::pair<uint64_t, uint64_t>(holeList[i].first + count, holeList[i].second);
          holeList.erase(holeList.begin() + i);
          holeList.insert(holeList.begin() + i, newHole);
          count = newHole.first;

        //Add another hole
        }else if((holeList[i].second + holeList[i].first * pageSize) < addr){
          newHole = std::pair<uint64_t, uint64_t>(count, addr);
          holeList.insert(holeList.begin() + i + 1, newHole);
        }

        //Replace hole with larger hole
        if((addr + (count + 1) * pageSize) == holeList[i].second)
        {
          newHole = std::pair<uint64_t, uint64_t>(holeList[i].first + count, addr);
          holeList.erase(holeList.begin() + i);
          holeList.insert(holeList.begin() + i, newHole);

        //Add another hole
        }else if((addr + count * pageSize) < holeList[i].second){
          newHole = std::pair<uint64_t, uint64_t>(count, addr);
          holeList.insert(holeList.begin() + i, newHole);
        }
        i++;
      }
    }
  }
}

bool PhysMemManager::allReleased(void) const
{
  return nAllocatedPages == 0;
}

uint64_t
PhysMemManager::getMaxAllocatedPages(void) const
{
  return maxAllocatedPages;
}
