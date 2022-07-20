/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    sys-status.h - System status module.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "sys-status.h"

#include <iostream>

SysStatus::SysStatus(const MemAddress base)
  : base{ base }
{
}

/*
 * MemoryInterface
 */

uint8_t
SysStatus::readByte(MemAddress addr)
{
  throw IllegalAccess("Not supported on sysstatus interface");
}

uint16_t
SysStatus::readHalfWord(MemAddress addr)
{
  throw IllegalAccess("Not supported on sysstatus interface");
}

uint32_t
SysStatus::readWord(MemAddress addr)
{
  throw IllegalAccess("Not supported on sysstatus interface");
}

uint64_t
SysStatus::readDoubleWord(MemAddress addr)
{
  throw IllegalAccess("Not supported on sysstatus interface");
}


void
SysStatus::writeByte(MemAddress addr, uint8_t value)
{
  if (addr != base + 0x8)
    throw IllegalAccess("Invalid system status address");

  std::cerr << "System halt requested." << std::endl;
  shouldHaltFlag = true;
}

void
SysStatus::writeHalfWord(MemAddress addr, uint16_t value)
{
  throw IllegalAccess("Not supported on sysstatus interface");
}

void
SysStatus::writeWord(MemAddress addr, uint32_t value)
{
  if (addr != base + 0x8)
    throw IllegalAccess("Invalid system status address");

  std::cerr << "System halt requested." << std::endl;
  shouldHaltFlag = true;
}

void
SysStatus::writeDoubleWord(MemAddress addr, uint64_t value)
{
  throw IllegalAccess("Not supported on sysstatus interface");
}

bool
SysStatus::contains(MemAddress addr) const
{
  return base <= addr && addr < base + 0x10;
}
