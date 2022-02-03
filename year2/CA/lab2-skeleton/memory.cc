/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory.h - A simple memory.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "memory.h"

#include <cstdlib>

Memory::Memory(const std::string &name,
               std::byte * const data,
               const MemAddress base,
               const size_t size,
               const size_t align)
  : name(name), base(base), size(size), align(align), data(data)
{
}

Memory::~Memory()
{
  /* Memory was allocated with alignment and nothrow, so we must
   * also deallocate this way.
   */
  operator delete[](data, std::align_val_t{ align }, std::nothrow);
}

void
Memory::setMayWrite(bool setting)
{
  mayWrite = setting;
}

/*
 * MemoryInterface
 */

template <typename T>
T
Memory::readData(MemAddress addr)
{
  if (! canAccess(addr, sizeof(T), false))
    throw IllegalAccess(addr, sizeof(T));

  MemAddress effectiveAddr = addr - base;

  return *reinterpret_cast<T *>(data + effectiveAddr);
}

uint8_t
Memory::readByte(MemAddress addr)
{
  return readData<uint8_t>(addr);
}

uint16_t
Memory::readHalfWord(MemAddress addr)
{
  return readData<uint16_t>(addr);
}

uint32_t
Memory::readWord(MemAddress addr)
{
  return readData<uint32_t>(addr);
}

uint64_t
Memory::readDoubleWord(MemAddress addr)
{
  return readData<uint64_t>(addr);
}


template <typename T>
void
Memory::writeData(MemAddress addr, T value)
{
  if (! canAccess(addr, sizeof(value), true))
    throw IllegalAccess(addr, sizeof(value));

  MemAddress effectiveAddr = addr - base;
  *reinterpret_cast<T *>(data + effectiveAddr) = value;
}

void
Memory::writeByte(MemAddress addr, uint8_t value)
{
  writeData(addr, value);
}

void
Memory::writeHalfWord(MemAddress addr, uint16_t value)
{
  writeData(addr, value);
}

void
Memory::writeWord(MemAddress addr, uint32_t value)
{
  writeData(addr, value);
}

void
Memory::writeDoubleWord(MemAddress addr, uint64_t value)
{
  writeData(addr, value);
}

bool
Memory::contains(MemAddress addr) const
{
  return base <= addr && addr < base + size;
}


/*
 * Private methods
 */
bool
Memory::canAccess(MemAddress addr, size_t accessSize, bool write) const
{
  if (addr < base || addr + accessSize > base + this->size)
    return false;

  if (write && !mayWrite)
    return false;

  return true;
}
