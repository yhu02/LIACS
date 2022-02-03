/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"
#include <string>
#include <functional>
#include <map>
#include <iostream>

std::ostream &
operator<<(std::ostream &os, const InstructionDecoder &decoder)
{

	std::string str;

	switch (decoder.control.getSignal())
	{
	case AUIPC:
		str = "AUIPC";
		break;
	case LUI:
		str = "LUI";
		break;
	case NOP:
		str = "NOP";
		break;
	case JAL:
		str = "JAL";
		break;
	case JALR:
		str = "JALR";
		break;
	case LB:
		str = "LB";
		break;
	case LH:
		str = "LH";
		break;
	case LW:
		str = "LW";
		break;
	case LD:
		str = "LD";
		break;
	case SD:
		str = "SD";
		break;
	case SB:
		str = "SB";
		break;
	case SH:
		str = "SH";
		break;
	case SW:
		str = "SW";
		break;
	case ADD:
		str = "ADD";
		break;
	case SUB:
		str = "SUB";
		break;
	case OR:
		str = "OR";
		break;
	case XOR:
		str = "XOR";
		break;
	case AND:
		str = "AND";
		break;
	case SLL:
		str = "SLL";
		break;
	case SRL:
		str = "SRL";
		break;
	case SRA:
		str = "SRA";
		break;
	case BEQ:
		str = "BEQ";
		break;
	case BNE:
		str = "BNE";
		break;
	case BGE:
		str = "BGE";
		break;
	case BGEU:
		str = "BGEU";
		break;
	case BLT:
		str = "BLT";
		break;
	case BLTU:
		str = "BLTU";
		break;
	case SLT:
		str = "SLT";
		break;
	case SLTU:
		str = "SLTU";
		break;
	}
	//Disregard field if the instruction doesnt contain the field

	os << "\nInstructionword | Instruction | RD | RS1 | RS2 | Immediate \n";
	os << decoder.getInstructionWord() << " | " << str << " | " << int(decoder.getRD());
	os << " | " << int(decoder.getRS1()) << " | " << int(decoder.getRS2()) << " | " << int(decoder.getImmediate());

	return os;
}
