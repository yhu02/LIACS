/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.h - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#ifndef __STAGES_H__
#define __STAGES_H__

#include "alu.h"
#include "mux.h"
#include "inst-decoder.h"
#include "memory-control.h"
#include "control-unit.h"
#include "hazard-detection-unit.h"
namespace STAGES
{
	extern int debugMode_g;
}
/* Pipeline registers may be read during propagate and may only be
 * written during clockPulse. Note that you cannot read the incoming
 * pipeline registers in clockPulse (e.g. in clockPulse of EX, you cannot
 * read ID_EX) because that register will already have been overwritten.
 * In case you need to propagate values from one pipeline register to
 * the next, these need to be buffered explicitly within the stage.
 */
struct IF_IDRegisters
{
	MemAddress PC = 0;
	RegValue instructionWord = 0;
};

struct ID_EXRegisters
{
	MemAddress PC{};
	/* TODO: add necessary fields */
	SIGNAL signal;
	TYPE type;
	ALUOp op;

	RegValue RD;
	RegValue RS1;
	RegValue RS2;
	RegValue Immediate;
};

struct EX_MRegisters
{
	MemAddress PC{};

	/* TODO: add necessary fields */

	SIGNAL signal;
	TYPE type;
	ALUOp op;
	RegValue RD;
	RegValue storeData;
	RegValue output{};
	RegValue offset;
};

struct M_WBRegisters
{
	MemAddress PC{};
	/* TODO: add necessary fields */
	RegValue RD;
	RegValue size;
	SIGNAL signal;
	TYPE type;
	ALUOp op;
	RegValue output{};
};

/*
 * Abstract base class for pipeline stage
 */

class Stage
{
public:
	Stage(bool pipelining)
		: pipelining(pipelining)
	{
	}

	virtual ~Stage()
	{
	}

	virtual void propagate() = 0;
	virtual void clockPulse() = 0;

protected:
	bool pipelining;
};

/*
 * Instruction fetch
 */

class InstructionFetchFailure : public std::exception
{
public:
	explicit InstructionFetchFailure(const MemAddress addr)
	{
		std::stringstream ss;
		ss << "Instruction fetch failed at address " << std::hex << addr;
		message = ss.str();
	}

	const char *what() const noexcept override
	{
		return message.c_str();
	}

private:
	std::string message{};
};

class TestEndMarkerEncountered : public std::exception
{
public:
	explicit TestEndMarkerEncountered(const MemAddress addr)
	{
		std::stringstream ss;
		ss << "Test end marker encountered at address " << std::hex << addr;
		message = ss.str();
	}

	const char *what() const noexcept override
	{
		return message.c_str();
	}

private:
	std::string message{};
};

class InstructionFetchStage : public Stage
{
public:
	InstructionFetchStage(bool pipelining,
						  IF_IDRegisters &if_id,
						  InstructionMemory instructionMemory,
						  MemAddress &PC, Mux<RegValue, MUXSELECTOR> &muxPC,
						  HazardDetectionUnit &hazard_detection_unit)
		: Stage(pipelining),
		  if_id(if_id), instructionMemory(instructionMemory),
		  PC(PC), muxPC(muxPC),
		  hazard_detection_unit(hazard_detection_unit)
	{
	}

	void propagate() override;
	void clockPulse() override;

private:
	IF_IDRegisters &if_id;

	InstructionMemory instructionMemory;
	MemAddress &PC;
	Mux<RegValue, MUXSELECTOR> &muxPC;
	HazardDetectionUnit &hazard_detection_unit;
};

/*
 * Instruction decode
 */

class InstructionDecodeStage : public Stage
{
public:
	InstructionDecodeStage(bool pipelining,
						   const IF_IDRegisters &if_id,
						   ID_EXRegisters &id_ex,
						   RegisterFile &regfile,
						   InstructionDecoder &decoder,
						   Mux<RegValue, MUXSELECTOR> &muxPC,
						   uint64_t &nInstrIssued,
						   uint64_t &nStalls,
						   HazardDetectionUnit &hazard_detection_unit)
		: Stage(pipelining),
		  if_id(if_id), id_ex(id_ex),
		  regfile(regfile), decoder(decoder),
		  nInstrIssued(nInstrIssued), nStalls(nStalls), muxPC(muxPC),
		  hazard_detection_unit(hazard_detection_unit)
	{
	}

	void propagate() override;
	void clockPulse() override;

private:
	const IF_IDRegisters &if_id;
	ID_EXRegisters &id_ex;

	RegisterFile &regfile;
	InstructionDecoder &decoder;

	uint64_t &nInstrIssued;
	uint64_t &nStalls;
	Mux<RegValue, MUXSELECTOR> &muxPC;
	HazardDetectionUnit &hazard_detection_unit;

	RegValue RD{};
	MemAddress PC{};
};

/*
 * Execute
 */

class ExecuteStage : public Stage
{
public:
	ExecuteStage(bool pipelining,
				 const ID_EXRegisters &id_ex,
				 EX_MRegisters &ex_m)
		: Stage(pipelining),
		  id_ex(id_ex), ex_m(ex_m)
	{
	}

	void propagate() override;
	void clockPulse() override;

private:
	const ID_EXRegisters &id_ex;
	EX_MRegisters &ex_m;

	MemAddress PC{};
	RegValue RD{};
	ALU alu{};
	Mux<RegValue, MUXSELECTOR> muxA{};
	Mux<RegValue, MUXSELECTOR> muxB{};

	SIGNAL signal{};
	RegValue RS2{};
	TYPE type{};
};

/*
 * Memory
 */

class MemoryStage : public Stage
{
public:
	MemoryStage(bool pipelining,
				const EX_MRegisters &ex_m,
				M_WBRegisters &m_wb,
				DataMemory dataMemory, Mux<RegValue, MUXSELECTOR> &muxPC)
		: Stage(pipelining),
		  ex_m(ex_m), m_wb(m_wb), dataMemory(dataMemory),
		  muxPC(muxPC)
	{
	}

	void propagate() override;
	void clockPulse() override;

private:
	const EX_MRegisters &ex_m;
	M_WBRegisters &m_wb;
	DataMemory dataMemory;

	Mux<RegValue, MUXSELECTOR> &muxPC;
	MemAddress PC{};

	RegValue output{};
	RegValue offset{};
	RegValue storeData{};

	SIGNAL signal{};
	RegValue RD{};
	TYPE type{};
};

class WriteBackStage : public Stage
{
public:
	WriteBackStage(bool pipelining,
				   const M_WBRegisters &m_wb,
				   RegisterFile &regfile,
				   uint64_t &nInstrCompleted,
				   HazardDetectionUnit &hazard_detection_unit)
		: Stage(pipelining),
		  m_wb(m_wb), regfile(regfile),
		  nInstrCompleted(nInstrCompleted),
		  hazard_detection_unit(hazard_detection_unit)
	{
	}

	void propagate() override;
	void clockPulse() override;
	bool flag = false;

private:
	const M_WBRegisters &m_wb;

	RegisterFile &regfile;
	uint64_t &nInstrCompleted;
	HazardDetectionUnit &hazard_detection_unit;
	TYPE type{};
	MemAddress RD{};
};

#endif /* __STAGES_H__ */
