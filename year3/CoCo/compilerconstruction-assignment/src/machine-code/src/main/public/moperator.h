/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_MOPERATOR_H
#define BUILD_MOPERATOR_H

#include "to_string.h"
#include <ostream>
#include <ioperator.h>

/**
 * This file contains an Enum to represent MachineCode-Operators
 * Also contains 'to_string' and ostream functionalities
 */

typedef enum moperator {
    /**
     * Assignment 1: Calculator
     */
    /* Meta */
    MOP_UNKNOWN,    // [--] (yet) Unknown Operation
    MOP_ERROR,      // [--] Error Operator

    /* Binary arithmetic operators
     *  src     source operand
     *  dst     destination operand */
    MOP_ADD,        // [sd] Addition (dst = dst + src)
    MOP_SUB,        // [sd] Subtraction (dst = dst - src)
    MOP_MUL,        // [sd] Multiplication (dst = dst * src)

    /* Unary arithmetic operators
     *  src     source operand */
    MOP_NEG,        // [s-] Unary Minus (src = 0-src)

    /* Move operator
     * src      source operand
     * dst      destination operand */
    MOP_MOVE,       // [sd] Move (dst = src)

    /* Print instruction */
    MOP_PRINT,      // [--] Print

    /**
     * Assignment 2: Dangling Else
     */

    /* Compare
     * src      first operand to compare
     * dst      second operand to compare */
    MOP_COMP,       // [sd] Compare

    /* Jump if Condition is Met - instructions
     * src      label to jump to */
    MOP_JE,         // [s-] Jump Equal
    MOP_JNE,        // [s-] Jump Not Equal
    MOP_JL,         // [s-] Jump Less
    MOP_JGE,        // [s-] Jump Greater Equal
    MOP_JLE,        // [s-] Jump Less Equal
    MOP_JG,         // [s-] Jump Greater

    /* One-Ops
     * src      operand */
    MOP_LABEL,      // [s-] Label
    MOP_JMP,        // [s-] Unconditional jump

    /**
     * Assignment 3: Types
     */
     MOP_EXTM,       // [sd] Sign-or-Zero-Extend Move

     /**
      * Assignment 4: Basic Compiler
      */
     MOP_XOR,        // [sd] XOR
     MOP_CONV,       // [--] Double Size (CWD, CDQ or CQO) RAX --> RAX:RDX, or (CBW: AL -> AX)
     MOP_DIV,        // [sd] Divide
     MOP_SHR,        // [sd] Shift Right (reg, imm)

} MOperator;

namespace util {
    template<>
    std::string to_string<MOperator>(MOperator);
}

typedef struct typed_moperator {
    MOperator mop = MOP_UNKNOWN;
    OperatorType type = OPT_VOID;
    bool is_signed = false;
} TypedMoperator;

inline std::ostream& operator<<(std::ostream& stream, const typed_moperator& tmop) {
    switch (tmop.mop) {
        case MOP_UNKNOWN: stream << "??"; break;
        case MOP_ERROR: stream << "err"; break;

        case MOP_SHR: stream << "shr"; break;

        /** Instructions depending on type */
        case MOP_ADD: stream << "add" << tmop.type; break;
        case MOP_SUB: stream << "sub" << tmop.type; break;
        case MOP_NEG: stream << "neg" << tmop.type; break;
        case MOP_MOVE: stream << "mov" << tmop.type; break;
        case MOP_COMP: stream << "cmp" << tmop.type; break;
        case MOP_MUL: stream << "imul" << tmop.type; break;

        case MOP_CONV:
            switch (tmop.type) {
                case OPT_BYTE: stream << "cbw"; break;
                case OPT_WORD: stream << "cwd"; break;
                case OPT_DOUBLE: stream << "cdq"; break;
                case OPT_QUAD: stream << "cqo"; break;
                default: stream << "??"; break;
            }
            break;


        /** Instructions not depending on type */
        case MOP_JE: stream << "je"; break;
        case MOP_JNE: stream << "jne"; break;

        case MOP_LABEL: stream << ".L"; break;
        case MOP_JMP: stream << "jmp"; break;

        case MOP_XOR: stream << "xor"; break;

        /** Jumps depending on signedness: less/ below and greater/ above
         * source: https://stackoverflow.com/a/27284932 */
        case MOP_JL: (tmop.is_signed) ? stream << "jl" : stream << "jb"; break;
        case MOP_JGE: (tmop.is_signed) ? stream << "jge" : stream << "jae"; break;
        case MOP_JLE: (tmop.is_signed) ? stream << "jle" : stream << "jbe"; break;
        case MOP_JG: (tmop.is_signed) ? stream << "jg": stream << "ja"; break;

        /** Other instructions depending on signedness */
        case MOP_EXTM: (tmop.is_signed) ? stream << "movsx": stream << "movzx"; break;

        case MOP_DIV: (tmop.is_signed) ? stream << "idiv" << tmop.type: stream << "div" << tmop.type; break;

        /** print one of four builtin-functions */
        case MOP_PRINT: {
            if (tmop.type == OPT_BYTE && tmop.is_signed)
                stream << "call\t\twriteint8";
            else if (tmop.type == OPT_BYTE && !tmop.is_signed)
                stream << "call\t\twriteuint8";
            else if (tmop.type == OPT_DOUBLE && tmop.is_signed)
                stream << "call\t\twriteinteger";
            else if (tmop.type == OPT_DOUBLE && !tmop.is_signed)
                stream << "call\t\twriteunsigned";
        }
    }
    return stream;
}


#endif //BUILD_MOPERATOR_H
