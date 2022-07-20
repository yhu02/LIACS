/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    serial.h - Dumb write-only serial interface.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "memory-interface.h"

class Serial : public MemoryInterface
{
  public:
    Serial(const MemAddress base);
    ~Serial() override = default;

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

  private:
    const MemAddress base;
};

#endif /* __SERIAL_H__ */
