/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    reg-file.h - Dual-ported register file.
 *
 * Copyright (C) 2016,2020  Leiden University, The Netherlands.
 */

#ifndef __REG_FILE__
#define __REG_FILE__

#include "arch.h"

#include <array>
#include <stdexcept>
#include <string>

class Processor;

/* For now hard-coded for a single zero-register and
 * (NumRegs - 1) general-purpose registers.
 */
class RegisterFile
{
public:
	RegisterFile() = default;

	/*
     * Input signals
     */

	void setRS1(const RegNumber newRS1) { RS1 = newRS1; };
	void setRS2(const RegNumber newRS2) { RS2 = newRS2; };

	void setRD(const RegNumber newRD) { RD = newRD; };
	void setWriteData(const RegValue newData) { writeData = newData; }
	void setWriteEnable(bool newEnable) { writeEnable = newEnable; }

	/*
     * Output signals
     */

	RegValue getReadData1() const
	{
		return readRegister(RS1);
	}

	RegValue getReadData2() const
	{
		return readRegister(RS2);
	}

	/*
     * Clock signal
     */

	void clockPulse()
	{
		if (writeEnable)
			writeRegister(RD, writeData);
	}

private:
	std::array<RegValue, NumRegs - 1> registers{};

	RegNumber RS1{};
	RegNumber RS2{};

	RegNumber RD{};
	RegValue writeData{};
	bool writeEnable = false;

	void checkRegNumber(const RegNumber regnum) const
	{
		if (regnum >= NumRegs)
		{
			throw std::out_of_range("register number " +
									std::to_string(regnum) +
									" out of range.");
		}
	}

	RegValue readRegister(const RegNumber regnum) const
	{
		checkRegNumber(regnum);

		if (regnum == 0)
			return 0;
		return registers[regnum - 1];
	}

	void writeRegister(const RegNumber regnum,
					   RegValue value)
	{
		checkRegNumber(regnum);

		if (regnum == 0)
			return;
		registers[regnum - 1] = value;
	}

	/* to allow access to read/writeRegister */
	friend Processor;
};

#endif /* __REG_FILE_H__ */
