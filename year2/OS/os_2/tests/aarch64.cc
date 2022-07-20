/* pagetables -- A framework to experiment with memory management
 *
 *    tests/simple.cc - unit tests for simple page table.
 *
 * Copyright (C) 2022  Leiden University, The Netherlands.
 */

#define BOOST_TEST_MODULE AArch64
#include <boost/test/unit_test.hpp>

#include "arch/include/aarch64.h"
using namespace AArch64;

constexpr static uint64_t MemorySize = (1 * 1024 * 1024) << 10;
constexpr static int entries = 1 << (addressSpaceBits - pageBits - 23);


BOOST_AUTO_TEST_SUITE(aarch64_test)

/*
 * Test MMU class separately
 */

struct MMUFixture
{
  AArch64MMU mmu;
  TableEntry *table;

  MMUFixture()
    : mmu(), table(nullptr)
  {
    /* Allocate and clear page table */
    table = new (std::align_val_t{ pageSize }, std::nothrow) TableEntry[entries];
    std::fill_n(table, entries, TableEntry{ 0 });

    mmu.setPageTablePointer(reinterpret_cast<uintptr_t>(table));
  }

  ~MMUFixture()
  {
    operator delete[](table, std::align_val_t{ pageSize}, std::nothrow);
  }

  MMUFixture(const MMUFixture &) = delete;
  MMUFixture &operator=(const MMUFixture &) = delete;
};


/* Test empty page table */
BOOST_FIXTURE_TEST_CASE( empty_page_table, MMUFixture )
{
  /* Traverse through address space in increments of 1/4 pages, all
   * translations should fail silently.
   */
  for (uintptr_t addr = 0x0; addr < 16 * pageSize; addr += pageSize / 4)
    {
      MemAccess access{ .type = MemAccessType::Load, .addr = addr, .size = 8 };
      uint64_t pAddr = 0;
      BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), false);
    }
}

/* Test whether MMU correctly translates page numbers */
BOOST_FIXTURE_TEST_CASE( mmu_translate_page_numbers, MMUFixture )
{
  table[0].valid = true;
  table[0].outputAddr = 0xf00;

  table[6].valid = true;
  table[6].outputAddr = 0xa00;

  /* Access virtual page 0x0, should translate to page 0xf00 */
  MemAccess access{ .type = MemAccessType::Load, .addr = 0, .size = 8 };
  uint64_t pAddr = 0;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), true);
  BOOST_CHECK_EQUAL(pAddr, 0xf00UL << pageBits);

  /* Access virtual page 0x1, should fail. */
  access.addr = 0x1 << pageBits;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), false);

  /* Access virtual page 0x6, should translate to page 0xa00 */
  access.addr = 0x6 << pageBits;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), true);
  BOOST_CHECK_EQUAL(pAddr, 0xa00UL << pageBits);
}

/* Test whether MMU correctly accounts for page offsets during address
 * translation.
 */
BOOST_FIXTURE_TEST_CASE( mmu_translate_page_offsets, MMUFixture )
{
  table[0].valid = true;
  table[0].outputAddr = 0xf00;

  table[6].valid = true;
  table[6].outputAddr = 0xa00;

  /* Access virtual page 0x0, should translate to page 0xf00 */
  MemAccess access{ .type = MemAccessType::Load, .addr = 1234, .size = 8 };
  uint64_t pAddr = 0;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), true);
  BOOST_CHECK_EQUAL(pAddr, (0xf00UL << pageBits) | 1234UL);

  /* Access virtual page 0x1, should fail. */
  access.addr = (0x1 << pageBits) | 5678UL;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), false);

  /* Access virtual page 0x6, should translate to page 0xa00 */
  access.addr = (0x6 << pageBits) | 1267UL;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), true);
  BOOST_CHECK_EQUAL(pAddr, (0xa00UL << pageBits) | 1267UL);
}

/*
 * Test MMUDriver; requires MMU to be instantiated.
 */

struct MMUDriverFixture
{
  AArch64MMU mmu;
  AArch64MMUDriver driver;

  Processor processor;
  ProcessList list = {};
  OSKernel kernel;

  MMUDriverFixture()
    : mmu(), driver(), processor(mmu),
    kernel(processor, driver, MemorySize, list)
  {
  }

  MMUDriverFixture(const MMUDriverFixture &) = delete;
  MMUDriverFixture &operator=(const MMUDriverFixture &) = delete;
};


BOOST_FIXTURE_TEST_CASE( instantiate, MMUDriverFixture )
{
  /* We only test whether the class composition can be successfully
   * instantiated.
   */
}

/* Test page table allocation for multiple PID */
BOOST_FIXTURE_TEST_CASE( page_table_allocation, MMUDriverFixture )
{
  constexpr uint64_t tableSize = sizeof(TableEntry) * entries;

  BOOST_CHECK_EQUAL(driver.getPageTable(0), 0x0);
  BOOST_CHECK_EQUAL(driver.getPageTable(1234), 0x0);

  driver.allocatePageTable(0);
  BOOST_CHECK_NE(driver.getPageTable(0), 0x0);
  BOOST_CHECK_EQUAL(driver.getPageTable(1234), 0x0);
  BOOST_CHECK_EQUAL(driver.getBytesAllocated(), 1 * tableSize);

  driver.allocatePageTable(1234);
  BOOST_CHECK_NE(driver.getPageTable(0), 0x0);
  BOOST_CHECK_NE(driver.getPageTable(1234), 0x0);
  BOOST_CHECK_EQUAL(driver.getBytesAllocated(), 2 * tableSize);

  driver.releasePageTable(0);
  BOOST_CHECK_EQUAL(driver.getPageTable(0), 0x0);
  BOOST_CHECK_NE(driver.getPageTable(1234), 0x0);

  driver.releasePageTable(1234);
  BOOST_CHECK_EQUAL(driver.getPageTable(0), 0x0);
  BOOST_CHECK_EQUAL(driver.getPageTable(1234), 0x0);

  BOOST_CHECK_EQUAL(driver.getBytesAllocated(), 2 * tableSize);
}

/* Test setting a mapping (adding an address translation entry).
 */
BOOST_FIXTURE_TEST_CASE( set_mapping, MMUDriverFixture )
{
  driver.allocatePageTable(0);
  processor.getMMU().setPageTablePointer(driver.getPageTable(0));

  /* Verify entry is initially not there. We call getTranslation twice
   * on purpose to ensure the first call does not add the mapping.
   */
  MemAccess access{ .type = MemAccessType::Load, .addr = 1234, .size = 8 };
  uint64_t pAddr = 0;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), false);
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), false);

  /* Now add the mapping and test whether address translation now succeeds */
  PhysPage pPage{ .PID = 0, .addr = 2 * pageSize };
  driver.setMapping(0, 0x0, pPage);
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), true);
  BOOST_CHECK_EQUAL(pAddr, (2 * pageSize) | 1234UL);

  /* Tear down */
  processor.getMMU().setPageTablePointer(0x0);
  driver.releasePageTable(0);
}

/* Test methods to get and manipulate the state of a physical page. */
BOOST_FIXTURE_TEST_CASE( page_state, MMUDriverFixture )
{
  driver.allocatePageTable(0);
  processor.getMMU().setPageTablePointer(driver.getPageTable(0));

  /* Add mapping for virtual address 0x0 */
  PhysPage pPage{ .PID = 0, .addr = 2 * pageSize };
  driver.setMapping(0, 0x0, pPage);

  /* Verify that when page is set to invalid, address translation will fail. */
  driver.setPageValid(pPage, false);
  MemAccess access{ .type = MemAccessType::Load, .addr = 1234, .size = 8 };
  uint64_t pAddr = 0;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), false);

  /* Tear down */
  processor.getMMU().setPageTablePointer(0x0);
  driver.releasePageTable(0);
}

/* Test whether page faults are handled and indeed add an address mapping. */
BOOST_FIXTURE_TEST_CASE( page_fault, MMUDriverFixture )
{
  driver.allocatePageTable(0);
  processor.getMMU().setPageTablePointer(driver.getPageTable(0));

  /* Verify mapping for address is not present */
  MemAccess access{ .type = MemAccessType::Load, .addr = 1234, .size = 8 };
  uint64_t pAddr = 0;
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), false);

  /* Process memory access; should trigger a page fault. */
  mmu.processMemAccess(access);

  /* A mapping for the same address must know be present */
  BOOST_CHECK_EQUAL(mmu.getTranslation(access, pAddr), true);

  /* Tear down */
  kernel.releaseMemory(reinterpret_cast<void *>(pAddr), pageSize);
  processor.getMMU().setPageTablePointer(0x0);
  driver.releasePageTable(0);
}

BOOST_AUTO_TEST_SUITE_END()
