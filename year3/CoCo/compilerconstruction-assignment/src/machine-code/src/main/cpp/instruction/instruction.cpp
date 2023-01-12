/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "instruction.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& stream, const Instruction& instruction) {
    const int op_width = 6;

    TypedMoperator mop = {instruction.mOperator, instruction.type, instruction.is_signed};

    OperatorType type = (instruction.mOperator == MOP_EXTM) ? OperatorType(instruction.type-1) : instruction.type;
    TypedMOperand src = {instruction.src, type};
    TypedMOperand dst = {instruction.dst, instruction.type};

    if (instruction.mOperator == MOP_JMP || instruction.mOperator == MOP_JE || instruction.mOperator == MOP_JNE
        || instruction.mOperator == MOP_JL || instruction.mOperator == MOP_JGE || instruction.mOperator == MOP_JLE
        || instruction.mOperator == MOP_JG) {
        stream << "\t\t"  << mop << std::setw(op_width) << "\t" << ".L" << int32_t(instruction.src.value.value);
        return stream;
    }
    if (instruction.mOperator == MOP_LABEL) {
        stream << ".L" << int32_t(instruction.src.value.value) << ":";
        return stream;
    }
    if (instruction.dst.flag == MOPF_NONE || instruction.src.flag == MOPF_NONE)
        stream << "\t\t" << mop << std::setw(op_width) << "\t" << src << dst;
    else
        stream << "\t\t" << mop << std::setw(op_width) << "\t" << src << ", " << dst;
    return stream;
}