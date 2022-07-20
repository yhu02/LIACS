/* pagetables -- A framework to experiment with memory management
 *
 *    simple.h - A simple, single level page table to serve as example.
 *
 * Copyright (C) 2017-2020 Leiden University, The Netherlands.
 */

#ifndef __ARCH_SIMPLE__
#define __ARCH_SIMPLE__

#include "mmu.h"
#include "oskernel.h"

#include <map>

namespace Simple {

/* Like in x86_64, we consider that only 48 bits of the virtual address
 * may be used.
 */
const static uint64_t addressSpaceBits = 48;


/* Table entry definitions and assorted constants. */

const static uint64_t pageBits = 26; /* 64 MiB / page */
const static uint64_t pageSize = 1UL << pageBits;

/* Page tables contain 2^22 entries and are 2^24 bytes in size.
 * log2(sizeof(TableEntry)) = 2.
 */
const static uint64_t pageTableAlign = 1UL << (addressSpaceBits - pageBits + 2);


struct __attribute__ ((__packed__)) TableEntry
{
  uint8_t valid : 1;
  uint8_t read : 1;
  uint8_t write : 1;

  uint8_t dirty : 1;
  uint8_t referenced : 1;

  uint16_t reserved : 13;

  /* In the initial implementation of "simple" it was decided that the
   * physical (output) address is 40 bits in size. This limits the amount
   * of physical memory to 1 TiB. With a page offset of 26 bits, this
   * results in a physical page number of 14 bits.
   */
  uint16_t physicalPage : 14;
};

/*
 * MMU hardware part (arch/simple/mmu.cc)
 */

class SimpleMMU : public MMU
{
  public:
    SimpleMMU();
    virtual ~SimpleMMU();

    virtual uint8_t getPageBits(void) const override
    {
      return pageBits;
    }

    virtual uint64_t getPageSize(void) const override
    {
      return pageSize;
    }

    virtual bool performTranslation(const uint64_t vPage,
                                    uint64_t &pPage,
                                    bool isWrite) override;
};


/*
 * OS driver part (arch/simple/driver.cc)
 */

class SimpleMMUDriver : public MMUDriver
{
  protected:
    std::map<uint64_t, TableEntry *> pageTables;
    uint64_t bytesAllocated;
    OSKernel *kernel;  /* no ownership */

  public:
    SimpleMMUDriver();
    virtual ~SimpleMMUDriver() override;

    virtual void      setHostKernel(OSKernel *kernel) override;

    virtual uint64_t  getPageSize(void) const override;

    virtual void      allocatePageTable(const uint64_t PID) override;
    virtual void      releasePageTable(const uint64_t PID) override;
    virtual uintptr_t getPageTable(const uint64_t PID) override;

    virtual void      setMapping(const uint64_t PID,
                                 uintptr_t vAddr,
                                 PhysPage &pPage) override;

    virtual void      setPageValid(PhysPage &pPage,
                                   bool setting) override;

    virtual uint64_t  getBytesAllocated(void) const override;

    /* Disallow objects from being copied, since it has a pointer member. */
    SimpleMMUDriver(const SimpleMMUDriver &driver) = delete;
    void operator=(const SimpleMMUDriver &driver) = delete;
};

} /* namespace Simple */

#endif /* __ARCH_SIMPLE__ */
