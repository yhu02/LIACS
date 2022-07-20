/* pagetables -- A framework to experiment with memory management
 *
 *    oskernel.cc - OS kernel component
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#include "oskernel.h"
#include "physmemmanager.h"
#include "settings.h"

#include <iostream>
#include <vector>


/*
 * MMUDriver (de)constructor required by linker.
 */

MMUDriver::MMUDriver()
{
}

MMUDriver::~MMUDriver()
{
}


/*
 * OSKernel
 */

OSKernel::OSKernel(Processor &processor, MMUDriver &driver,
                   uint64_t memorySize, ProcessList &processList)
  : manager(std::make_unique<PhysMemManager>(driver.getPageSize(), memorySize)),
    processor(processor), driver(driver),
    processList(processList), current(nullptr), pPages{},
    nPageFaults(0), nContextSwitches(0)
{
  driver.setHostKernel(this);

  /* Allocate root page tables for all processes. */
  for (auto &p : processList)
    driver.allocatePageTable(p->getPID());

  /* Configure processor: set page fault handler ("exception routine"),
   * timer interrupt interval and interrupt handler.
   */
  using std::placeholders::_1;
  processor.getMMU().initialize(std::bind(&OSKernel::pageFaultHandler,
                                          this, _1 ));

  processor.setTimerInterval(ProcessTimeQuantum);

  processor.setInterruptHandler(std::bind(&OSKernel::interruptHandler,
                                          this, _1 ));
}

OSKernel::~OSKernel()
{
  if (current != nullptr)
    {
      processor.setProcess(nullptr);
      processList.push_back(current);
    }

  /* Terminate all processes that remain. */
  for (auto &p : processList)
    terminateProcess(p->getPID());

  driver.setHostKernel(nullptr);

  /* Check all allocated memory was released. */
  if (not manager->allReleased())
    std::cerr << std::endl
              << "Warning: not all physical pages were released!" << std::endl;

  std::cerr << std::dec << std::endl
            << "Statistics:" << std::endl
            << "# context switches: " << getNContextSwitches() << std::endl
            << "# handled page faults: " << getNPageFaults() << std::endl
            << "# bytes allocated for page tables: "
            << driver.getBytesAllocated() << std::endl
            << "max. # allocated physical pages: "
            << getMaxAllocatedPages() << std::endl;
}

void *
OSKernel::allocateMemory(size_t size, uint64_t align)
{
  const uint64_t pageSize = driver.getPageSize();

  /* Round up size to multiple of pageSize. Wasteful approach, but works.
  */
  size = (size + (pageSize - 1)) & ~(pageSize - 1);

  /* Currently don't support alignment on multiples larger than
  * page size.
  */
  
  if (align > pageSize)
    throw std::runtime_error("cannot allocate memory: unsupported alignment.");
  
  uintptr_t addr;
  if (not manager->allocatePages(size / pageSize, addr))
    throw std::runtime_error("cannot allocate memory: memory full.");

  return (void *)addr;
}

void
OSKernel::releaseMemory(void *ptr, size_t size)
{
  const uint64_t pageSize = driver.getPageSize();

  /* Round up size to multiple of pageSize. Wasteful approach, but works.
   */
  size = (size + (pageSize - 1)) & ~(pageSize - 1);

  manager->releasePages(reinterpret_cast<uintptr_t>(ptr),
                        size / pageSize);
}


void
OSKernel::terminateProcess(const uint64_t PID)
{
  for(size_t i = 0; i < pPages.size(); i++)
  {
    if(pPages[i].PID == PID)
    {
      manager->releasePages(reinterpret_cast<uint64_t>(pPages[i].driverData), 1);
    }
  }
  std::cerr << "KERNEL: process " << PID << " has finished." << std::endl;
    
  /* Ask driver to release page tables */
  driver.releasePageTable(PID);
}

void
OSKernel::pageFaultHandler(const uint64_t faultAddr)
{
  logPageFault(faultAddr);

  /* Determine virtual page on which the fault occurred. */
  uint64_t vAddr = faultAddr & ~(driver.getPageSize() - 1);

  /* Allocate physical page and ask the driver to set the
   * virtual to physical mapping for this page.
   */
  PhysPage pPage;
  if (not manager->allocatePages(1, pPage.addr))
    {
      throw std::runtime_error("Physical memory full.");
    }
  pPage.PID = current->getPID();
  driver.setMapping(current->getPID(), vAddr, pPage);
  pPages.push_back(pPage);

  logPageMapping(vAddr, pPage.addr);
}

void
OSKernel::interruptHandler(InterruptRequest request)
{
  if (request == InterruptRequest::SyscallExit)
    {
      terminateProcess(current->getPID());
    }
  else
    {
      /* Process has not completed, so put back on the ready queue. */
      if (current != nullptr)
        processList.push_back(current);
    }

  /* Select a new process to run using round robin scheduling. */

  /* Front of the queue (if any) is the next to run. */
  std::shared_ptr<Process> prev = current;
  if (processList.size() == 0)
    current = nullptr;
  else
    {
      current = processList.front();
      processList.pop_front();
    }

  if (current != prev)
    {
      /* Perform context switch; change root page table pointer */
      if (current != nullptr)
        {
          uintptr_t table = driver.getPageTable(current->getPID());
          processor.getMMU().setPageTablePointer(table);
        }

      // Flush on context switch if ASID is disabled, we dont want processes
      // accessing other processes memory
      if(!UseASID){
        processor.getMMU().getTLB()->flush();
      }
      processor.setProcess(current);
      
      nContextSwitches++;
      std::cerr << std::hex << std::showbase
          << "KERNEL: context switch: now executing " << current->getPID()
          << std::endl;
    }
}

int
OSKernel::getNPageFaults() const
{
  return nPageFaults;
}

int
OSKernel::getNContextSwitches() const
{
  return nContextSwitches;
}

int
OSKernel::getMaxAllocatedPages() const
{
  return manager->getMaxAllocatedPages();
}


void
OSKernel::logPageFault(const uint64_t faultAddr)
{
  std::cerr << "VM: handling page fault for address "
            << std::hex << std::showbase << faultAddr << std::endl;
  nPageFaults++;
}

void
OSKernel::logPageMapping(const uint64_t virtualAddr,
                         const uint64_t physicalAddr)
{
  std::cerr << std::hex << std::showbase
            << "VM: virtual page " << virtualAddr
            << " has been mapped to physical page "
            << physicalAddr
            << std::endl;
}
