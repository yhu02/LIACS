/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_INSTRUCTION_H
#define BUILD_INSTRUCTION_H

#include <cstdint>
#include <ioperator.h>
#include "moperator.h"
#include "moperand.h"

/**
 * This file contains a class to represent the MachineCode-Instructions
 * An Instructions consists of the following:
 *      mOperator   src     dst
 * The instructions are also printable through the << operator
 */

class Instruction {
public:
    Instruction(OperatorType type, bool is_signed, MOperator mOperator, MOperand src, MOperand dst) :
        type(type), is_signed(is_signed), mOperator(mOperator), src(src), dst(dst) {}
    static Instruction generate_error_instruction() { return {OPT_VOID, false, MOP_ERROR, error_moperand(), error_moperand()}; }
    friend std::ostream& operator<<(std::ostream& stream, const Instruction& instruction);
private:
    OperatorType type;
    bool is_signed = false;
    MOperator mOperator;
    MOperand src;
    MOperand dst;
};




#endif //BUILD_INSTRUCTION_H
