/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-control.cc - Memory Controller
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */
#include "memory-control.h"

InstructionMemory::InstructionMemory(MemoryBus &bus)
	: bus(bus), size(0), addr(0)
{
}

void InstructionMemory::setSize(const uint8_t size)
{
	if (size != 2 and size != 4)
		throw IllegalAccess("Invalid size " + std::to_string(size));

	this->size = size;
}

void InstructionMemory::setAddress(const MemAddress addr)
{
	this->addr = addr;
}

RegValue
InstructionMemory::getValue() const
{
	switch (size)
	{
	case 2:
		return bus.readHalfWord(addr);

	case 4:
		return bus.readWord(addr);

	default:
		throw IllegalAccess("Invalid size " + std::to_string(size));
	}
}

DataMemory::DataMemory(MemoryBus &bus)
	: bus{bus}
{
}

void DataMemory::setSize(const uint8_t size)
{
	if (size == 1 || size == 2 || size == 4 || size == 8)
	{
		this->size = size;
	}
	else
	{
		throw IllegalAccess("Invalid size " + std::to_string(size));
	}
}

void DataMemory::setAddress(const MemAddress addr)
{

	this->addr = addr;
}

void DataMemory::setDataIn(const RegValue value)
{
	this->dataIn = value;
}

void DataMemory::setReadEnable(bool setting)
{
	readEnable = setting;
}

void DataMemory::setWriteEnable(bool setting)
{
	writeEnable = setting;
}

RegValue
DataMemory::getDataOut(bool signExtend) const
{
	if (readEnable)
	{
		switch (size)
		{
		case 1:
			return bus.readByte(addr);

		case 2:
			return bus.readHalfWord(addr);

		case 4:
			return bus.readWord(addr);

		case 8:
			return bus.readDoubleWord(addr);

		default:
			throw IllegalAccess("Invalid size " + std::to_string(size));
		}
	}
	else
	{
		throw IllegalAccess("Read not enabled");
	}
	return 0;
}

void DataMemory::clockPulse() const
{

	if (writeEnable)
	{
		switch (size)
		{
		case 1:
			bus.writeByte(addr, dataIn);
			break;

		case 2:
			bus.writeHalfWord(addr, dataIn);
			break;

		case 4:
			bus.writeWord(addr, dataIn);
			break;
		case 8:
			bus.writeDoubleWord(addr, dataIn);
			break;

		default:
			throw IllegalAccess("Invalid size " + std::to_string(size));
		}
		
	}
	else
	{
		throw IllegalAccess("Write not enabled");
	}
}
