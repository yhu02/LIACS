/* pagetables -- A framework to experiment with memory management
 *
 *    physmemmanager.h - Physical Memory Manager
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#ifndef __PHYSMEMMANAGER_H__
#define __PHYSMEMMANAGER_H__

#include "settings.h"

#include <vector>

/* We assume that the RAM starts at 4GB in the physical address space.
 */
const static uint64_t physMemBase = 4UL*1024*1024*1024;


class PhysMemManager
{
  protected:
    void *baseAddress;
    const uint64_t pageSize;
    const uint64_t memorySize;

    uint64_t nPages;
    uint64_t nAllocatedPages;
    uint64_t maxAllocatedPages;
    std::vector<bool> allocated;
    std::vector<std::pair<uint64_t, uint64_t>> holeList;

  public:
    PhysMemManager(const uint64_t pageSize, const uint64_t memorySize);
    ~PhysMemManager();

    bool      allocatePages(size_t count, uintptr_t &addr);
    void      releasePages(uintptr_t addr, size_t count);

    bool      allReleased(void) const;
    uint64_t  getMaxAllocatedPages(void) const;

    PhysMemManager(const PhysMemManager &) = delete;
    PhysMemManager &operator=(const PhysMemManager &) = delete;
};

#endif /* __PHYSMEMMANAGER_H__ */
