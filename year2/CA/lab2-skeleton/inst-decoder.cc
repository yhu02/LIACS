/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */
#include "inst-decoder.h"

#include <functional>
#include <map>

#define ADDRESSBITS 5

/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

//returns twos complement of num and uses MSB as most significant bit
RegValue
twosComplement(size_t MSB, RegValue num)
{
	if ((1 << (MSB - 1)) & num)
	{
		RegValue val = -1;
		val <<= MSB;
		val += num;
		return val;
	}
	return num;
}
//returns the bits at and between the positions start and end and shifts them to the right
uint32_t
getBits(size_t start, size_t end, uint32_t instructionWord)
{
	instructionWord <<= 31 - start;
	instructionWord >>= 31 - start + end;

	return instructionWord;
}

void InstructionDecoder::setInstructionWord(const uint32_t instructionWord)
{
	this->instructionWord = instructionWord;
}

uint32_t
InstructionDecoder::getInstructionWord() const
{
	return instructionWord;
}

RegNumber
InstructionDecoder::getFunct7() const
{
	return getBits(31, 25, instructionWord);
}

RegNumber
InstructionDecoder::getRS1() const
{
	return getBits(19, 15, instructionWord);
}

RegNumber
InstructionDecoder::getRS2() const
{
	return getBits(24, 20, instructionWord);
}

RegNumber
InstructionDecoder::getFunct3() const
{
	return getBits(14, 12, instructionWord);
}

RegNumber
InstructionDecoder::getRD() const
{
	return getBits(11, 7, instructionWord);
}

RegNumber
InstructionDecoder::getOpcode() const
{
	return getBits(6, 0, instructionWord);
}

RegValue
InstructionDecoder::getImmediate() const
{
	RegValue immediate = 0;

	if (control.getType() == TYPE::I)
	{
		//Size 12
		immediate = getBits(31, 20, instructionWord);

		immediate = twosComplement(12, immediate);
	}
	else if (control.getType() == TYPE::S)
	{
		//Size 12
		RegValue imm11_5 = getBits(31, 25, instructionWord);
		RegValue imm4_0 = getBits(11, 7, instructionWord);

		imm11_5 <<= 5;
		immediate = imm11_5 + imm4_0;
		immediate = twosComplement(12, immediate);
	}
	else if (control.getType() == TYPE::B)
	{
		//Size 13
		RegValue imm12 = getBits(31, 31, instructionWord);
		RegValue imm10_5 = getBits(30, 25, instructionWord);
		RegValue imm4_1 = getBits(11, 8, instructionWord);
		RegValue imm11 = getBits(7, 7, instructionWord);

		imm12 <<= 12;
		imm10_5 <<= 5;
		imm11 <<= 11;
		imm4_1 <<= 1;

		immediate = imm12 + imm10_5 + imm4_1 + imm11;
		immediate = twosComplement(13, immediate);
	}
	else if (control.getType() == TYPE::U)
	{
		//Size 20
		immediate = getBits(31, 12, instructionWord);
		immediate = twosComplement(20, immediate);
	}
	else if (control.getType() == TYPE::J)
	{
		//Size 21
		RegValue imm20 = getBits(31, 31, instructionWord);
		RegValue imm10_1 = getBits(30, 21, instructionWord);
		RegValue imm11 = getBits(20, 20, instructionWord);
		RegValue imm19_12 = getBits(19, 12, instructionWord);

		imm20 <<= 20;
		imm10_1 <<= 1;
		imm11 <<= 11;
		imm19_12 <<= 12;

		immediate = imm20 + imm10_1 + imm11 + imm19_12;
		immediate = twosComplement(21, immediate);
	}

	return immediate;
}