/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.cc - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */
#include "unistd.h"
#include "stages.h"

#include <iostream>

/*
 * Instruction decode
 */
int STAGES::debugMode_g = 0;

void dump_instruction(RegValue a, RegValue b, RegValue c, RegValue d, RegValue e, const void *separator1, const void *separator2, const void *separator3)
{
	if (STAGES::debugMode_g == 2)
	{
		write(1, separator1, 1);
		write(1, std::to_string(a).c_str(), std::to_string(a).size());
		write(1, separator3, 1);
		write(1, separator1, 1);
		write(1, std::to_string(b).c_str(), std::to_string(b).size());
		write(1, separator2, 1);
		write(1, std::to_string(c).c_str(), std::to_string(c).size());
		write(1, separator3, 1);
		write(1, separator1, 1);
		write(1, std::to_string(d).c_str(), std::to_string(d).size());
		write(1, separator2, 1);
		write(1, std::to_string(e).c_str(), std::to_string(e).size());
		write(1, separator3, 1);
		write(1, "\n", 1);
	}
}

/*
 * Instruction fetch
 */

void InstructionFetchStage::propagate()
{
	PC = muxPC.getOutput();
	muxPC.setInput(I1, PC + 4);
	muxPC.setSelector(I1);

	try
	{
		instructionMemory.setAddress(PC);
		instructionMemory.setSize(4);

		uint32_t instructionWord = instructionMemory.getValue();

		if (instructionWord == TestEndMarker)
		{
			throw TestEndMarkerEncountered(PC);
		}
	}
	catch (TestEndMarkerEncountered &e)
	{
		throw;
	}
	catch (std::exception &e)
	{
		throw InstructionFetchFailure(PC);
	}
}

void InstructionFetchStage::clockPulse()
{
	if_id.PC = PC;
	if_id.instructionWord = instructionMemory.getValue();
}

/*
 * Decode
 */

void InstructionDecodeStage::propagate()
{
	decoder.setInstructionWord(if_id.instructionWord);
	decoder.control.setControl(decoder.getFunct7(), decoder.getFunct3(), decoder.getOpcode());

	if (decoder.getType() == TYPE::R || decoder.getType() == TYPE::S || decoder.getType() == TYPE::B || decoder.getType() == TYPE::I)
	{
		regfile.setRS1(decoder.getRS1()); //set address rs1
		hazard_detection_unit.setRS1(decoder.getRS1());
	}
	if (decoder.getType() == TYPE::R || decoder.getType() == TYPE::S || decoder.getType() == TYPE::B)
	{
		regfile.setRS2(decoder.getRS2()); //set address rs2
		hazard_detection_unit.setRS2(decoder.getRS2());
	}

	/* debug mode: dump decoded instructions to cerr.
   * In case of no pipelining: always dump.
   * In case of pipelining: special case, if the PC == 0x0 (so on the
   * first cycle), don't dump an instruction. This avoids dumping a
   * dummy instruction on the first cycle when ID is effectively running
   * uninitialized.
   */
	if (STAGES::debugMode_g && (!pipelining || (pipelining && PC != 0x0)))
	{
		/* Dump program counter & decoded instruction in debug mode */
		auto storeFlags(std::cerr.flags());

		std::cerr << "\n";
		std::cerr << std::hex << std::showbase << PC << "\t";
		std::cerr.setf(storeFlags);

		std::cerr << decoder << std::endl;
	}
	dump_instruction(if_id.PC, decoder.getFunct7(), decoder.getFunct3(), decoder.getOpcode(), decoder.getImmediate(), "{", ":", "}");

	PC = if_id.PC;
}

void InstructionDecodeStage::clockPulse()
{
	/* ignore the "instruction" in the first cycle. */
	if (!pipelining || (pipelining && PC != 0x0))
		++nInstrIssued;

	id_ex.PC = PC;

	if (hazard_detection_unit.foundHazard())
	{
		id_ex.RS1 = 0;
		id_ex.RS2 = 0;
		id_ex.RD = 0;

		id_ex.Immediate = 0;
		id_ex.signal = SIGNAL::ADD;
		id_ex.type = TYPE::R;
		id_ex.op = ALUOp::ADD_A;

		muxPC.setInput(I4, id_ex.PC);
		muxPC.setSelector(I4);
	}
	else
	{
		id_ex.RS1 = regfile.getReadData1();
		id_ex.RS2 = regfile.getReadData2();
		id_ex.RD = decoder.getRD();

		id_ex.Immediate = decoder.getImmediate();
		id_ex.signal = decoder.getSignal();
		id_ex.type = decoder.getType();
		id_ex.op = decoder.getOp();
		if (id_ex.type == TYPE::J || id_ex.type == TYPE::B)
		{
			hazard_detection_unit.registers.push_back(0);
		}
		else
		{
			hazard_detection_unit.push_back(decoder.getRD());
		}
	}
}

/*
 * Execute
 */

void ExecuteStage::propagate()
{
	muxA.setInput(I1, id_ex.RS1);
	muxA.setInput(I2, id_ex.Immediate << 12);
	muxA.setInput(I3, id_ex.Immediate);

	muxB.setInput(I1, id_ex.Immediate);
	muxB.setInput(I2, id_ex.RS2);
	muxB.setInput(I3, id_ex.PC);
	muxB.setInput(I4, 0);

	if (id_ex.type == TYPE::I || id_ex.type == TYPE::S) //I-type or S-type
	{
		muxA.setSelector(I1);
		muxB.setSelector(I1);
	}
	else if (id_ex.type == TYPE::R || id_ex.type == TYPE::B) //R-type or B-type
	{
		muxA.setSelector(I1);
		muxB.setSelector(I2);

		if (id_ex.type == TYPE::B) //branch
		{
			ex_m.offset = id_ex.Immediate;
		}
	}
	else if (id_ex.type == TYPE::U)
	{
		muxA.setSelector(I2);
		if (id_ex.signal == 0b10000001) //AUIPC
		{
			muxB.setSelector(I3);
		}
		else if (id_ex.signal == 0b11000001) //LUI
		{
			muxB.setSelector(I4);
		}
	}
	else if (id_ex.type == TYPE::J)
	{
		if (id_ex.signal & 0b100000000) //JAL
		{
			muxA.setSelector(I3);
			muxB.setSelector(I3);
		}
	}

	if (id_ex.signal & 0b1111) //ALU operation
	{
		alu.setA(muxA.getOutput());
		alu.setB(muxB.getOutput());
		alu.setOp(id_ex.op);

		dump_instruction(id_ex.PC, muxA.getOutput(), muxB.getOutput(), alu.getResult(), id_ex.RD, "(", ":", ")");
	}

	PC = id_ex.PC;
	RD = id_ex.RD;
	signal = id_ex.signal;
	RS2 = id_ex.RS2;
	type = id_ex.type;
}

void ExecuteStage::clockPulse()
{
	ex_m.PC = PC;
	ex_m.RD = RD;
	ex_m.output = alu.getResult();
	ex_m.signal = signal;
	ex_m.storeData = RS2;
	ex_m.type = type;
}

/*
 * Memory
 */

void MemoryStage::propagate()
{
	if (ex_m.signal & 0b110000) // load/store operation
	{
		size_t size = 1;
		size <<= ((0b111 & ex_m.signal) - 1); //1, 2, 4, 8 size of data in bytes

		if (ex_m.signal & 0b10000) //load
		{
			dataMemory.setAddress(ex_m.output);
			dataMemory.setSize(size);
			dataMemory.setReadEnable(true);
		}
		else if (ex_m.signal & 0b100000) //store
		{
			dataMemory.setSize(size);
			dataMemory.setDataIn(ex_m.storeData);
			dataMemory.setAddress(ex_m.output);
			dataMemory.setWriteEnable(true);
		}
	}

	muxPC.setInput(I5, ex_m.output);
	muxPC.setInput(I3, ex_m.offset + ex_m.PC);
	if (ex_m.signal & 0b100000000) //JAL or JALR  jump
	{
		muxPC.setSelector(I5);
	}
	else if (ex_m.type == TYPE::B && ex_m.output == 1) //Branch jump
	{
		muxPC.setSelector(I3);
	}

	PC = ex_m.PC;
	type = ex_m.type;
	signal = ex_m.signal;
	RD = ex_m.RD;
	output = ex_m.output;

	dump_instruction(ex_m.PC, ex_m.output, ex_m.RD, 0, 0, "<", ":", ">");
}

void MemoryStage::clockPulse()
{
	if (signal & 0b10000) //load
	{
		m_wb.output = dataMemory.getDataOut(1);
		dataMemory.setReadEnable(false);
	}
	else if (signal & 0b100000) //store
	{
		dataMemory.clockPulse();
		dataMemory.setWriteEnable(false);
	}
	else
	{
		m_wb.output = output; //result from alu operation
	}

	m_wb.PC = PC;
	m_wb.signal = signal;
	m_wb.type = type;
	m_wb.RD = RD;
}

/*
 * Write back
 */

void WriteBackStage::propagate()
{
	if (!pipelining || (pipelining && m_wb.PC != 0x0))
		++nInstrCompleted;

	if (m_wb.type != TYPE::S && m_wb.type != TYPE::B) //check if not store operation
	{
		if (m_wb.signal & 0b00011111)
		{
			if (m_wb.signal == 0b100000001) //JAL
			{
				regfile.setWriteData(m_wb.PC + 4); //save value to register
			}
			else
			{
				regfile.setWriteData(m_wb.output); //save value to register
			}
			regfile.setRD(m_wb.RD);
			regfile.setWriteEnable(true);
		}
	}
	RD = m_wb.RD;
	type = m_wb.type;
	dump_instruction(m_wb.PC, m_wb.output, m_wb.RD, 0, 0, "[", ":", "]");
}

void WriteBackStage::clockPulse()
{
	if (type == TYPE::B || type == TYPE::J)
	{
		hazard_detection_unit.erase(0);
	}
	else
	{
		hazard_detection_unit.erase(RD);
	}

	regfile.clockPulse();
	regfile.setWriteEnable(false);
}
