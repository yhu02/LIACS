/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    sys-status.h - System status module.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

/* The system status module currently only supports halting the
 * system. Other functionalities could be implemented here at different
 * memory addresses. (For instance, reading the instruction counter,
 * number of clock cycles since start up, etc.).
 */

#ifndef __SYS_STATUS_H__
#define __SYS_STATUS_H__

#include "memory-interface.h"

class SysStatus : public MemoryInterface
{
  public:
    SysStatus(const MemAddress base);
    ~SysStatus() override = default;

    bool shouldHalt() const { return shouldHaltFlag; }

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

    bool shouldHaltFlag = false;
};

#endif /* __SYS_STATUS_H__ */
