/* pagetables -- A framework to experiment with memory management
 *
 *    simple.h - A simple, single level page table to serve as example.
 *
 * Copyright (C) 2017-2020 Leiden University, The Netherlands.
 */

#ifndef __A_ARCH_64__
#define __A_ARCH_64__

#include "mmu.h"
#include "oskernel.h"

#include <map>
#include <iostream>
namespace AArch64 {

/* Like in x86_64, we consider that only 48 bits of the virtual address
 * may be used.
 */
const static uint64_t addressSpaceBits = 48;


/* Table entry definitions and assorted constants. */

const static uint64_t pageBits = 14; /* 16 KiB / page */
const static uint64_t pageSize = 1UL << pageBits;

/* Page tables contain 2^22 entries and are 2^24 bytes in size.
 * log2(sizeof(TableEntry)) = 2.
 */
const static uint64_t pageTableAlign = 1UL << (addressSpaceBits - pageBits + 3 - 23);


struct __attribute__ ((__packed__)) TableEntry
{
  uint64_t outputAddr : 34;
  uint16_t upperAttr : 12;
  uint8_t valid : 1;
  uint8_t type : 1;
  uint8_t attrIndex : 3;
  uint8_t nonSecure : 1;
  uint8_t accessPermission : 2;
  uint8_t shareability : 2;
  uint8_t accessFlag : 1;
  uint8_t notGlobal : 1;
  uint8_t dirtyBitModifier : 1;
  uint8_t reserved : 3;
  uint8_t reserved2 : 2;

};

/*
 * MMU hardware part (arch/simple/mmu.cc)
 */

class AArch64MMU : public MMU
{
  public:
    AArch64MMU();
    virtual ~AArch64MMU();

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

class AArch64MMUDriver : public MMUDriver
{
  protected:
    std::map<uint64_t, TableEntry *> pageTables;
    uint64_t bytesAllocated;
    OSKernel *kernel;  /* no ownership */

  public:
    AArch64MMUDriver();
    virtual ~AArch64MMUDriver() override;

    virtual void      setHostKernel(OSKernel *kernel) override;

    virtual uint64_t  getPageSize(void) const override;

    virtual void      allocatePageTable(const uint64_t PID) override;
    
    virtual TableEntry*  allocatePageTable2();

    virtual void      releasePageTable(const uint64_t PID) override;

    virtual uintptr_t getPageTable(const uint64_t PID) override;

    virtual void      setMapping(const uint64_t PID,
                                 uintptr_t vAddr,
                                 PhysPage &pPage) override;

    virtual void      setPageValid(PhysPage &pPage,
                                   bool setting) override;

    virtual uint64_t  getBytesAllocated(void) const override;

    /* Disallow objects from being copied, since it has a pointer member. */
    AArch64MMUDriver(const AArch64MMUDriver &driver) = delete;
    void operator=(const AArch64MMUDriver &driver) = delete;
};

} /* namespace Simple */

#endif /* __ARCH_SIMPLE__ */
