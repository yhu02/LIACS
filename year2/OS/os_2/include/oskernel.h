/* pagetables -- A framework to experiment with memory management
 *
 *    oskernel.h - OS kernel component
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#ifndef __OSKERNEL_H__
#define __OSKERNEL_H__

#include "processor.h"
#include "process.h"

class OSKernel;
class PhysMemManager;

/* Structure representing a physical page allocated to a process. */
struct PhysPage
{
  uint64_t PID;
  uintptr_t addr;   /* physical page address */

  /* To be used by MMUDriver to be able to quickly locate page table
   * entry that corresponds to this physical page.
   */
  void *driverData;
};


class MMUDriver
{
  public:
    MMUDriver();
    virtual ~MMUDriver();

    virtual void      setHostKernel(OSKernel *kernel) = 0;

    /* Return the page size for this architecture */
    virtual uint64_t  getPageSize(void) const = 0;

    /* Allocate a (root) page table for the given PID. */
    virtual void      allocatePageTable(const uint64_t PID) = 0;

    /* Release page table (and allocated pages) for the given PID. */
    virtual void      releasePageTable(const uint64_t PID) = 0;

    /* Return the root page table for the given PID. */
    virtual uintptr_t getPageTable(const uint64_t PID) = 0;

    /* An implementation of this method should add the specified
     * virtual to physical mapping to the specified page table.
     */
    virtual void      setMapping(const uint64_t PID,
                                 uintptr_t vAddr,
                                 PhysPage &pPage) = 0;

    /* Set the valid bit for the specified physical page. */
    virtual void      setPageValid(PhysPage &pPage,
                                   bool setting) = 0;

    /* Return number of bytes that has been allocated to store
     * page tables.
     */
    virtual uint64_t  getBytesAllocated(void) const = 0;
};


/* The OS kernel is only concerned with allocating physical memory and
 * creating page table mappings in response to page faults. We do not
 * deal with page permissions, virtual address space maps, and so on to
 * keep things simple.
 */
class OSKernel
{
  protected:
    std::unique_ptr<PhysMemManager> manager;
    Processor &processor;
    MMUDriver &driver;
    ProcessList &processList;
    std::shared_ptr<Process> current;
    std::vector<PhysPage> pPages;

    int nPageFaults;
    int nContextSwitches;
    


    void logPageFault(const uint64_t faultAddr);
    void logPageMapping(const uint64_t virtualAddr,
                        const uint64_t physicalAddr);

  public:
    OSKernel(Processor &processor, MMUDriver &driver,
             uint64_t memorySize, ProcessList &processList);
    virtual ~OSKernel();

    void *allocateMemory(size_t size, uint64_t align);
    void releaseMemory(void *ptr, size_t size);

    void terminateProcess(const uint64_t PID);

    void pageFaultHandler(const uint64_t faultAddr);
    void interruptHandler(InterruptRequest request);

    /* Getters for statistics */
    int getNPageFaults() const;
    int getNContextSwitches() const;
    int getMaxAllocatedPages() const;
};

#endif /* __OSKERNEL_H__ */
