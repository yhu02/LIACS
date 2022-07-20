#ifndef __CONTROL_UNIT_H__
#define __CONTROL_UNIT_H__

#include "unistd.h"
#include "alu.h"
#include "arch.h"
            //0b1111 designated for ALUop unless it is a load operation then it is shift amount for load size
            //0b10000 load operation
            //0b100000 store operation
            //0b1000000 branch operation
            //0b10000000 set operation
            //0b100000000
enum SIGNAL
{
	AUIPC	= 0b10000001,
	LUI 	= 0b11000001,

	NOP 	= 0b0,
	JAL 	= 0b100000001,
	JALR 	= 0b110000001,
	LB 		= 0b00010001,
	LH 		= 0b00010010,
	LW 		= 0b00010011,
	LD 		= 0b00010100,
	LBU 	= 0b00010001,
	LHU 	= 0b00010010,
	LWU 	= 0b00010011,
	SD 		= 0b00100100,
	SB 		= 0b00100001,
	SH 		= 0b00100010,
	SW 		= 0b00100011,

	ADDI 	= 0b00000001,
	ADD 	= 0b00000001,
	ADDIW 	= 0b00000001,
	ADDW 	= 0b00000001,
	SUB 	= 0b00000010,
	SUBW 	= 0b00000010,

	OR 		= 0b00000011,
	ORI 	= 0b00000011,
	XOR 	= 0b00000100,
	XORI 	= 0b00000100,
	ANDI 	= 0b00000101,
	AND 	= 0b00000101,

	SLL 	= 0b00000110,
	SLLI 	= 0b00000110,
	SLLIW 	= 0b00000110,
	SLLW 	= 0b00000110,
	SRL		= 0b00000111,
	SRLI 	= 0b00000111,
	SRLIW 	= 0b00000111,
	SRLW 	= 0b00000111,
	SRA 	= 0b00001000,
	SRAI 	= 0b00001000,
	SRAIW 	= 0b00001000,
	SRAW	= 0b00001000,

	BEQ 	= 0b01001001,
	BNE 	= 0b01001010,

	BGE 	= 0b01001011,
	BGEU 	= 0b01001100,
	BLT 	= 0b01001101,
	SLTI 	= 0b10001101,
	SLT 	= 0b10001101,
	SLTU 	= 0b10001110,
	SLTIU 	= 0b10001110,
	BLTU 	= 0b01001110,

	FENCE 	= 0b00000000,
	FENCEI 	= 0b00000000,
	ECALL 	= 0b00000000,
	EBREAK 	= 0b00000000,
	CSRRW 	= 0b00000000,
	CSRRS 	= 0b00000000,
	CSRRC 	= 0b00000000,
	CSRRWI 	= 0b00000000,
	CSRRSI 	= 0b00000000,
	CSRRCI 	= 0b00000000
};

enum class TYPE
{
	R,
	I,
	S,
	B,
	U,
	J
};

class ControlUnit
{
public:
	ControlUnit() = default;
	~ControlUnit() = default;

	void setControl(RegNumber funct7, RegNumber funct3, RegNumber opcode);
	SIGNAL getSignal() const
	{
		return signal;
	}
	TYPE getType() const
	{
		return type;
	}
	ALUOp getOp() const
	{
		return op;
	}

private:
	SIGNAL signal;
	TYPE type;
	ALUOp op;
};

#endif