/* pagetables -- A framework to experiment with memory management
 *
 *    mmu.h - Memory Management Unit component
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __MMU_H__
#define __MMU_H__

#include <functional>
#include <list>
#include <utility>
#include <unordered_map>
#include <algorithm>

#include "process.h" /* for MemAccess */


using PageFaultFunction = std::function<void(uintptr_t)>;

class MMU
{
  
//Table Lookaside Buffer implementation using LRU algorithm
  class TLB_LRU 
  {
   // store keys of TLB entries
      std::list<std::pair<uint64_t, std::pair<uint64_t, uint8_t>>> dq{};

      // store references of key in TLB
      std::unordered_map<uint64_t, std::list<std::pair<uint64_t, std::pair<uint64_t, uint8_t>>>::iterator> ma{};

      uint32_t size; // max amount of TLB entries

    public:
      TLB_LRU(int s): size(s), nLookups{0}, nHits{0}, nEvictions{0}, nFlush{0}, nFlushEvictions{0} {}

      void flush();
      void insert(std::pair<uint64_t, std::pair<uint64_t, uint8_t>>);
      uint64_t find(const uint8_t &asid, const uint64_t &vPage, bool &valid);
      int* moveStats();

    private:
      int nLookups;
      int nHits;
      int nEvictions;
      int nFlush;
      int nFlushEvictions;
  };

  protected:
    uintptr_t root;
    PageFaultFunction pageFaultHandler;
    TLB_LRU tlb;
    std::unordered_map<uint64_t, uint8_t> asidTable;

  public:
    MMU();
    virtual ~MMU();

    TLB_LRU* getTLB(){return &tlb;};
    void initialize(PageFaultFunction pageFaultHandler);
    void setPageTablePointer(const uintptr_t root);
    void processMemAccess(const MemAccess &access);

    uint64_t makePhysicalAddr(const MemAccess &access, const uint64_t pPage);
    bool getTranslation(const MemAccess &access, uint64_t &pAddr);

    /* This method is used to acquire statistics from the TLB implementation */
    void getTLBStatistics(int &nLookups, int &nHits,
                          int &nEvictions,
                          int &nFlush, int &nFlushEvictions);

    /* These methods should return the architecture's page size / bits. */
    virtual uint8_t getPageBits(void) const = 0;
    virtual uint64_t getPageSize(void) const = 0;

    /* An implementation of the "performTranslation" method should translate
     * the given virtual page *number* to a physical page number. We
     * consider a page number to be the address with the page offset
     * shifted away. We use page numbers instead of full addresses to
     * simplify the implementation of a TLB.
     */
    virtual bool performTranslation(const uint64_t vPage,
                                    uint64_t &pPage,
                                    bool isWrite) = 0;
};


#endif /* __MMU_H__ */
