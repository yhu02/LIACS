#include "control-unit.h"

void ControlUnit::setControl(RegNumber funct7, RegNumber funct3, RegNumber opcode)
{
	if (opcode == 0b0110111)
	{
		type = TYPE::U;
		signal = LUI;
	}
	else if (opcode == 0b0010111)
	{
		type = TYPE::U;
		signal = AUIPC;
	}
	else if (opcode == 0b1101111)
	{
		type = TYPE::J;
		signal = JAL;
	}
	else if (opcode == 0b1100111)
	{
		type = TYPE::I;
		if (funct3 == 0b000)
		{
			signal = JALR;
		}
	}
	else if (opcode == 0b1100011)
	{
		type = TYPE::B;
		if (funct3 == 0b000)
		{
			signal = BEQ;
		}
		else if (funct3 == 0b001)
		{
			signal = BNE;
		}
		else if (funct3 == 0b100)
		{
			signal = BLT;
		}
		else if (funct3 == 0b101)
		{
			signal = BGE;
		}
		else if (funct3 == 0b110)
		{
			signal = BLTU;
		}
		else if (funct3 == 0b111)
		{
			signal = BGEU;
		}
	}
	else if (opcode == 0b0000011)
	{
		type = TYPE::I;
		if (funct3 == 0b000)
		{
			signal = LB;
		}
		else if (funct3 == 0b001)
		{
			signal = LH;
		}
		else if (funct3 == 0b010)
		{
			signal = LW;
		}
		else if (funct3 == 0b100)
		{
			signal = LBU;
		}
		else if (funct3 == 0b101)
		{
			signal = LHU;
		}
		else if (funct3 == 0b110)
		{
			signal = LWU;
		}
		else if (funct3 == 0b011)
		{
			signal = LD;
		}
	}
	else if (opcode == 0b0100011)
	{
		type = TYPE::S;
		if (funct3 == 0b000)
		{
			signal = SB;
		}
		else if (funct3 == 0b001)
		{
			signal = SH;
		}
		else if (funct3 == 0b010)
		{
			signal = SW;
		}
		else if (funct3 == 0b011)
		{
			signal = SD;
		}
	}
	else if (opcode == 0b0010011)
	{
        
		type = TYPE::I;
		if (funct3 == 0b000)
		{
			signal = ADDI;
		}
		else if (funct3 == 0b010)
		{
			signal = SLTI;
		}
		else if (funct3 == 0b011)
		{
			signal = SLTIU;
		}
		else if (funct3 == 0b100)
		{
			signal = XORI;
		}
		else if (funct3 == 0b110)
		{
			signal = ORI;
		}
		else if (funct3 == 0b111)
		{
			signal = ANDI;
		}
		else if (funct3 == 0b001)
		{
			if (funct7 == 0b0000000)
			{
				signal = SLLI;
			}
		}
		else if (funct3 == 0b101)
		{
			if (funct7 == 0b0000000)
			{
				signal = SRLI;
			}
			else if (funct7 == 0b0100000)
			{
				signal = SRAI;
			}
		}
	}
	else if (opcode == 0b0110011)
	{
		type = TYPE::R;
		if (funct3 == 0b000)
		{
			if (funct7 == 0b0000000)
			{
				signal = ADD;
			}
			else if (funct7 == 0b0100000)
			{
				signal = SUB;
			}
		}
		else if (funct3 == 0b001)
		{
			if (funct7 == 0b0000000)
			{
				signal = SLL;
			}
		}
		else if (funct3 == 0b010)
		{
			if (funct7 == 0b0000000)
			{
				signal = SLT;
			}
		}
		else if (funct3 == 0b011)
		{
			if (funct7 == 0b0000000)
			{
				signal = SLTU;
			}
		}
		else if (funct3 == 0b100)
		{
			if (funct7 == 0b0000000)
			{
				signal = XOR;
			}
		}
		else if (funct3 == 0b101)
		{
			if (funct7 == 0b0000000)
			{
				signal = SRL;
			}
			else if (funct7 == 0b0100000)
			{
				signal = SRA;
			}
		}
		else if (funct3 == 0b110)
		{
			if (funct7 == 0b0000000)
			{
				signal = OR;
			}
		}
		else if (funct3 == 0b111)
		{
			if (funct7 == 0b0000000)
			{
				signal = AND;
			}
		}
	}
	else if (opcode == 0b0011011)
	{
		type = TYPE::I;
		if (funct3 == 0b000)
		{
			signal = ADDIW;
		}
		else if (funct3 == 0b001)
		{
			signal = SLLIW;
		}
		else if (funct3 == 0b101)
		{
			if (funct7 == 0b0000000)
			{
				signal = SRLIW;
			}
			else if (funct7 == 0b0100000)
			{
				signal = SRAIW;
			}
		}
	}
	else if (opcode == 0b0111011)
	{
		type = TYPE::R;
		if (funct3 == 0b000)
		{
			if (funct7 == 0b0000000)
			{
				signal = ADDW;
			}
			else if (funct7 == 0b0100000)
			{
				signal = SUBW;
			}
		}
		else if (funct3 == 0b001)
		{
			if (funct7 == 0b0000000)
			{
				signal = SLLW;
			}
		}
		else if (funct3 == 0b101)
		{
			if (funct7 == 0b0000000)
			{
				signal = SRLW;
			}
			else if (funct7 == 0b0100000)
			{
				signal = SRAW;
			}
		}
	}
	else if (opcode == 0b0001111)
	{
		///////////////////////////////////////////////Type???
		if (funct3 == 0b000)
		{
			signal = FENCE;
		}
		else if (funct3 == 0b001)
		{
			signal = FENCEI;
		}
	}
	else if (opcode == 0b1110011)
	{
		type = TYPE::I;
		if (funct3 == 0b000)
		{
			if (funct7 == 0b0000000)
			{
				signal = ECALL;
			}
			else if (funct7 == 0b0000001)
			{
				signal = EBREAK;
			}
		}
		else if (funct3 == 0b001)
		{
			signal = CSRRW;
		}
		else if (funct3 == 0b001)
		{
			signal = CSRRS;
		}
		else if (funct3 == 0b001)
		{
			signal = CSRRC;
		}
		else if (funct3 == 0b001)
		{
			signal = CSRRWI;
		}
		else if (funct3 == 0b001)
		{
			signal = CSRRSI;
		}
		else if (funct3 == 0b001)
		{
			signal = CSRRWI;
		}
	}
	else
	{
		signal = NOP;
	}
	if (signal & 0b110000) //Load and store use add
	{
		op = ALUOp(0b0001);
	}
	else
	{
		op = ALUOp(signal & 0b1111);
	}
};
