/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    processor.h - Processor class tying all components together.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include "arch.h"

#include "elf-file.h"
#include "stages.h"
#include "memory-control.h"
#include "sys-status.h"
#include "hazard-detection-unit.h"


class Processor
{
  public:
    Processor(ELFFile &program, bool pipelining, int debugMode=0);

    Processor(const Processor &) = delete;
    Processor &operator=(const Processor &) = delete;

    /* Command-line register initialization */
    void initRegister(RegNumber regnum, RegValue value);
    RegValue getRegister(RegNumber regnum) const;

    /* Instruction execution steps */
    bool run(bool testMode=false);

    /* Debugging and statistics */
    void dumpRegisters() const;
    void dumpStatistics() const;

  private:
    bool pipelining;

    /* Statistics */
    uint64_t nCycles{};
    uint64_t nInstrIssued{};
    uint64_t nInstrCompleted{};
    uint64_t nStalls{};

    /* Stages */
    std::vector<std::unique_ptr<Stage>> stages{};

    /* Components shared by multiple stages or components. */
    RegisterFile regfile{};
    InstructionDecoder decoder{};
    HazardDetectionUnit hazard_detection_unit{};

    MemoryBus bus;

    MemAddress PC{};

    /* Pipeline registers */
    IF_IDRegisters if_id{};
    ID_EXRegisters id_ex{};
    EX_MRegisters  ex_m{};
    M_WBRegisters  m_wb{};

    Mux<RegValue, MUXSELECTOR> muxPC{};

    /* Memory bus clients */
    SysStatus *sysStatus{};  /* no ownership */
};

#endif /* __PROCESSOR_H__ */
