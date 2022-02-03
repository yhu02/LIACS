/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    framebuffer.h - a simple framebuffer for the simulator
 *
 * Copyright (C) 2019  Leiden University, The Netherlands.
 *
 * Authors:
 *     Koen Putman
 */

#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "memory-interface.h"

#include <memory>

class RenderContext;

struct ControlInterface
{
  uint32_t enable;
  uint32_t mode;
  uint32_t resx;
  uint32_t resy;
};

enum class FBzone
{
  INVALID = 0,
  CONTROL,
  PALETTE,
  BUFFER
};


class Framebuffer : public MemoryInterface
{
  public:
    Framebuffer(const MemAddress control_base,
                const MemAddress framebuffer_base);
    ~Framebuffer() override;

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

    void processEvents(const bool redraw);



  private:
    FBzone getZone(const MemAddress addr, const uint8_t size,
                   uint32_t *offset) const;

    const MemAddress control_base;
    const MemAddress framebuffer_base;

    bool  active_window = false;
    bool  finished = false;

    uint64_t update_freq = 1000000;
    uint64_t cycles_since_update{};

    ControlInterface control{};
    std::unique_ptr<RenderContext> context;
};

#endif /* __FRAMEBUFFER_H__ */
