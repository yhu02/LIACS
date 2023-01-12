/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "moperator.h"

#include <ostream>

template<>
std::string util::to_string<MOperator>(MOperator op) {
    switch (op) {
        case MOP_UNKNOWN: return "Unknown M-Operator";
        case MOP_ERROR: return "Error M-Operator";
        case MOP_ADD: return "Addition";
        case MOP_SUB: return "Subtraction";
        case MOP_MUL: return "Multiplication";
        case MOP_NEG: return "Negation";
        case MOP_MOVE: return "Move";
        case MOP_COMP: return "Compare";
        case MOP_JE: return "Jump Equal";
        case MOP_JNE: return "Jump Not Equal";
        case MOP_JL: return "Jump Less";
        case MOP_JGE: return "Jump Greater Equal";
        case MOP_JLE: return "Jump Less Equal";
        case MOP_JG: return "Jump Greater";
        case MOP_LABEL: return "Label";
        case MOP_JMP: return "Unconditional Jump";
        case MOP_PRINT: return "Print";
        case MOP_EXTM: return "Sign-or-Zero-Extend Move";
        case MOP_XOR: return "XOR";
        case MOP_CONV: return "Double size from RAX";
        case MOP_DIV: return "Divide";
        case MOP_SHR: return "Shift Right";
    }
    return "??";
}