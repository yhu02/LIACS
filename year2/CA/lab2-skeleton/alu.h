/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ALU_H__
#define __ALU_H__

#include "arch.h"

#include <map>

enum ALUOp
{
	NOP_A =0b0000,
	ADD_A =0b0001,
	SUB_A =0b0010,
	OR_A  =0b0011,
	XOR_A =0b0100,
	AND_A =0b0101,
	SLL_A =0b0110,
	SRL_A =0b0111,
	SRA_A =0b1000,
	EQ_A  =0b1001,
	NE_A  =0b1010,
	GE_A  =0b1011,
	GEU_A =0b1100,
	LT_A  =0b1101,
	LTU_A =0b1110
};

/* The ALU component performs the specified operation on operands A and B
 * when asked to propagate the result. The operation is specified through
 * the ALUOp.
 */
class ALU
{
public:
	ALU();

	void setA(RegValue A) { this->A = A; }
	void setB(RegValue B) { this->B = B; }
	void setOp(ALUOp op) { this->op = op; }
	RegValue getResult();

private:
	int64_t A;
	int64_t B;

	ALUOp op;
};

#endif /* __ALU_H__ */
