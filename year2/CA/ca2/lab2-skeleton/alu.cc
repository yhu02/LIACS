/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */
#include "unistd.h"
#include "alu.h"

#include "inst-decoder.h"

#ifdef _MSC_VER
/* MSVC intrinsics */
#include <intrin.h>
#endif

ALU::ALU()
	: A(), B(), op()
{
}

RegValue
ALU::getResult()
{
	switch (op)
	{
	case NOP_A:
		return 0;
	case ADD_A:
		return A + B;
	case SUB_A:
		return A - B;
	case OR_A:
		return A | B;
	case XOR_A:
		return A ^ B;
	case AND_A:
		return A & B;
	case SLL_A:
		return A << B;
	case SRL_A:
		return (unsigned long long)(A) >> B;
	case SRA_A:
		return A >> B;
	case EQ_A:
		return A == B;
	case NE_A:
		return A != B;
	case GE_A:
		return A >= B;
	case GEU_A:
		return (unsigned long long)(A) >= (unsigned long long)(B);
	case LT_A:
		return A < B;
	case LTU_A:
		return (unsigned long long)(A) < (unsigned long long)(B);

	default:
		throw IllegalInstruction("Unimplemented or unknown ALU operation");
	}
	return 0;
}
