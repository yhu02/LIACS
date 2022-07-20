/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    mux.h - Trivial multiplexer class
 *
 * Copyright (C) 2020  Leiden University, The Netherlands.
 */

#ifndef __MUX_H__
#define __MUX_H__

#include <array>
#include <stdexcept>

/*
 * This class models a simple multiplexer:
 *
 *            +-----+
 *      ------|     |
 *            |  M  |
 *  values ---|  U  |----- output
 * (inputs)   |  X  |
 *      ------|     |
 *            +-----+
 *               |
 *               |
 *            selector
 *
 * The "selector" is used to select one of the values to appear on the
 * output signal. "V" should be the type of the values, "S" should be
 * the typename of an enum that will be used as selector. This enum must
 * contain an entry "LAST" as the final item. For example:
 *
 *     Mux<RegValue, InputSelector> myMultiplexer;
 *
 * Where InputSelector is defined as follows:
 *
 *     enum class InputSelector
 *     {
 *        InputOne,
 *        InputTwo,
 *        ...
 *        LAST
 *     };
 */
enum MUXSELECTOR
{
	I1,
	I2,
	I3,
	I4,
	I5,
	LAST
};

template <typename V, typename S>
class Mux
{
public:
	void setInput(S input, V value)
	{
		if (input == S::LAST)
			throw std::out_of_range("Cannot set LAST mux input.");

		values[static_cast<int>(input)] = value;
	}

	void setSelector(S selector)
	{
		if (selector == S::LAST)
			throw std::out_of_range("Cannot set LAST as mux selector.");

		this->selector = selector;
	}

	S getSelector() const
	{
		return selector;
	}

	V getOutput() const
	{
		return values[static_cast<int>(selector)];
	}

private:
	S selector;
	std::array<V, static_cast<size_t>(S::LAST)> values;
};

#endif /* __MUX_H__ */
