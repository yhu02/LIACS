/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    processor.h - Processor class tying all components together.
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "processor.h"
#include "inst-decoder.h"
#include "serial.h"
#include "framebuffer.h"

#include <iostream>
#include <iomanip>

Processor::Processor(ELFFile &program, bool pipelining, int debugMode)
	: pipelining{pipelining},
	  bus{program.createMemories()}
{
	bus.addClient(std::make_unique<Serial>(0x200));

	auto status = std::make_unique<SysStatus>(0x270);
	sysStatus = status.get();
	bus.addClient(std::move(status));

#ifdef ENABLE_FRAMEBUFFER
	bus.addClient(std::make_unique<Framebuffer>(0x800, 0x1000000));
#endif

	/* Stages */
	/* TODO: this might need modification in case the stages need access
   * to more shared components.
   */
	stages.emplace_back(std::make_unique<InstructionFetchStage>(pipelining,
																if_id,
																InstructionMemory(bus),
																PC, muxPC,
																hazard_detection_unit));
	stages.emplace_back(std::make_unique<InstructionDecodeStage>(pipelining,
																 if_id, id_ex,
																 regfile,
																 decoder, muxPC,
																 nInstrIssued,
																 nStalls,
																 hazard_detection_unit));
	stages.emplace_back(std::make_unique<ExecuteStage>(pipelining,
													   id_ex, ex_m));
	stages.emplace_back(std::make_unique<MemoryStage>(pipelining,
													  ex_m, m_wb,
													  DataMemory(bus), muxPC));
	stages.emplace_back(std::make_unique<WriteBackStage>(pipelining,
														 m_wb,
														 regfile,
														 nInstrCompleted,
														 hazard_detection_unit));

	/* Initialize PC */
	PC = program.getEntrypoint();
	STAGES::debugMode_g = debugMode;
	muxPC.setInput(I1, PC);
	muxPC.setSelector(I1);
}

/* This method is used to initialize registers using values
 * passed as command-line argument.
 */
void Processor::initRegister(RegNumber regnum, RegValue value)
{
	regfile.writeRegister(regnum, value);
}

RegValue
Processor::getRegister(RegNumber regnum) const
{
	return regfile.readRegister(regnum);
}

/* Processor main loop. Each iteration should execute an instruction.
 * One step in executing and instruction takes 1 clock cycle.
 *
 * The return value indicates whether an error (exception) occurred during
 * execution (false) or whether the program was executed without problems
 * (true).
 *
 * In "testMode" instruction fetch failures are not fatal. This is because
 * a clean shutdown of the program requires the store instruction to be
 * implemented, so that the system status module can be informed. In unit
 * tests, we want to test as little instructions as possible and thus allow
 * test programs without store instruction to run without error.
 */

bool Processor::run(bool testMode)
{
	while (!sysStatus->shouldHalt())
	{
		try
		{
			/* The "bus clock" runs at 1/5 the frequency of the Processor. */
			if (nCycles % 5 == 0)
				bus.clockPulse();

			if (!pipelining)
			{
				/* Perform the five instruction execution steps in sequence,
               * one step per cycle. This completes a single instruction.
               */
				for (auto &s : stages)
				{
					s->propagate();
					s->clockPulse();
					++nCycles;
				}
			}
			else
			{
				/* Run all stages within a single clock cycle. */
				for (auto &s : stages)
					s->propagate();
				for (auto &s : stages)
					s->clockPulse();

				++nCycles;
			}
		}
		catch (TestEndMarkerEncountered &e)
		{
			if (testMode)
				return true;
			/* else */
			std::cerr << "ABNORMAL PROGRAM TERMINATION; PC = "
					  << std::hex << PC << std::dec << std::endl;
			std::cerr << "Reason: " << e.what() << std::endl;
			return false;
		}
		catch (InstructionFetchFailure &e)
		{
			if (testMode)
				return true;
			/* else */
			std::cerr << "ABNORMAL PROGRAM TERMINATION; PC = "
					  << std::hex << PC << std::dec << std::endl;
			std::cerr << "Reason: " << e.what() << std::endl;
			return false;
		}
		catch (std::exception &e)
		{
			/* Catch exceptions such as IllegalInstruction and InvalidAccess */
			std::cerr << "ABNORMAL PROGRAM TERMINATION; PC = "
					  << std::hex << PC << std::dec << std::endl;
			std::cerr << "Reason: " << e.what() << std::endl;
			return false;
		}
		//this->dumpRegisters();
	}

	return true;
}

void Processor::dumpRegisters() const
{
	constexpr size_t NumColumns = 2;
	constexpr size_t valueFieldWidth = 18;
	auto storeFlags(std::cerr.flags());

	for (size_t i = 0; i < NumRegs / NumColumns; ++i)
	{
		std::cerr << "R" << std::setw(2) << std::setfill('0') << i << " "
				  << std::setw(valueFieldWidth)
				  << std::hex << std::showbase
				  << regfile.readRegister(i)
				  << "\t";
		std::cerr.setf(storeFlags);
		std::cerr << "R" << std::setw(2) << (i + NumRegs / NumColumns) << " "
				  << std::setw(valueFieldWidth)
				  << std::hex << std::showbase
				  << regfile.readRegister(i + NumRegs / NumColumns)
				  << std::endl;
		std::cerr.setf(storeFlags);
	}
}

void Processor::dumpStatistics() const
{
	std::cerr << nCycles << " clock cycles, "
			  << nInstrIssued << " instructions issued, "
			  << nInstrCompleted << " instructions completed." << std::endl;
	if (pipelining)
		std::cerr << nStalls << " stall cycles inserted." << std::endl;
	std::cerr << bus.getBytesRead() << " bytes read, "
			  << bus.getBytesWritten() << " bytes written." << std::endl;
}
