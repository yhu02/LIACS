/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory.h - A simple memory.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "memory-interface.h"

#include <memory>
#include <string>

class Memory : public MemoryInterface
{
  public:
    /* Constructor transfers ownership of data to this new object. */
    Memory(const std::string &name,
           std::byte *data,
           const MemAddress base,
           const size_t size,
           const size_t align);
    ~Memory() override;

    void setMayWrite(bool setting);

    /* MemoryInterface */
    uint8_t readByte(MemAddress addr) override;
    uint16_t readHalfWord(MemAddress addr) override;
    uint32_t readWord(MemAddress addr) override;
    uint64_t readDoubleWord(MemAddress addr) override;

    void writeByte(MemAddress addr, uint8_t value) override;
    void writeHalfWord(MemAddress addr, uint16_t value) override;
    void writeWord(MemAddress addr, uint32_t value) override;
    void writeDoubleWord(MemAddress addr, uint64_t value) override;

    bool contains(MemAddress addr) const override;


    Memory(const Memory &) = delete;
    Memory &operator=(const Memory &) = delete;

  private:
    const std::string name;

    /* Assume memory may always be read. Do not maintain
     * separate execute bit.
     */
    bool mayWrite = false;

    const MemAddress base;
    const size_t size;
    const size_t align;

    /* We are being passed in dynamically-aligned memory areas which
     * are allocated using C functions (to save trouble). So we don't
     * bother using a unique_ptr in this case.
     */
    std::byte * const data;

    /* Private helper methods */
    bool canAccess(MemAddress addr, size_t accessSize, bool write) const;

    template <typename T>
    T readData(MemAddress addr);
    template <typename T>
    void writeData(MemAddress addr, T value);
};

#endif /* __MEMORY_H__ */
