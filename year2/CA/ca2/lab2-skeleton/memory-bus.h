/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-bus.h - "Interconnect" for different memory bus clients.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __MEMORY_BUS_H__
#define __MEMORY_BUS_H__

#include "memory-interface.h"

#include <memory>
#include <vector>

class MemoryBus : public MemoryInterface
{
  public:
    MemoryBus(std::vector<std::unique_ptr<MemoryInterface> > &&clients);
    ~MemoryBus() override;

    void addClient(std::unique_ptr<MemoryInterface> client);

    uint64_t getBytesRead() const;
    uint64_t getBytesWritten() const;

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

    void clockPulse() override;

  private:
    std::vector<std::unique_ptr<MemoryInterface> > clients;

    MemoryInterface *findClient(MemAddress addr) noexcept;
    MemoryInterface *getClient(MemAddress addr);

    uint64_t bytesRead = 0;     /* Bytes read from bus */
    uint64_t bytesWritten = 0;  /* Bytes written to bus */
};

#endif /* __MEMORY_BUS_H__ */
