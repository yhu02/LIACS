/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    arch.h - Definitions specifying architectural properties.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ARCH_H__
#define __ARCH_H__

#include <cstdint>
#include <cstddef>

/* For now these values are hard-coded for a 64-bit RISC-V
 * architecture.
 */
using RegValue = uint64_t;
using MemAddress = uint64_t;

static constexpr size_t NumRegs = 32;
using RegNumber = uint8_t;
static constexpr size_t MaxRegs = 256;

/* Magic codeword, which is an invalid RISC-V instruction, that we use
 * to mark the end of unit test cases.
 */
static constexpr uint32_t TestEndMarker = 0xddffccff;


#endif /* __ARCH_H__ */
