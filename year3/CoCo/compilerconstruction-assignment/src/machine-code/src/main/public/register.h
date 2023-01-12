/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_REGISTER_H
#define BUILD_REGISTER_H

#include <ostream>
#include "ioperator.h"

/**
 * File contains an Enum to represent registers
 * Also contains streaming functionalities
 */


// Note: (https://stackoverflow.com/questions/15191178/how-do-ax-ah-al-map-onto-eax)
// writing into AL, AH, AX, leaves other bytes unmodified, writing EAX, zero-extends into RAX
typedef enum registername {
    // Name, convention
    RN_INVALID = 0, // invalid register
    RN_RAX, // return values, caller-save
    RN_RBX, // storing data related to RAX computation, callee-save
    RN_RCX, // counter, fourth argument, caller-save
    RN_RDX, // third argument, caller-save
    RN_RSI, // second argument, caller-save
    RN_RDI, // first argument, caller-save
    RN_RBP, // base pointer, callee-save
    RN_RSP, // stack pointer, caller-save
    RN_R8, // fifth argument, caller-save
    RN_R9, // sixth argument, caller-save
    RN_R10, // caller-save
    RN_R11, // caller-save
    RN_R12, // callee-save
    RN_R13, // callee-save
    RN_R14, // callee-save
    RN_R15, // callee-save
} RegisterName;

/** Returns base of register */
inline std::ostream& operator<<(std::ostream& stream, const registername& reg) {
    switch (reg) {
        case RN_INVALID: stream << "??"; break;
        case RN_RAX: stream << "a"; break;
        case RN_RBX: stream << "b"; break;
        case RN_RCX: stream << "c"; break;
        case RN_RDX: stream << "d"; break;
        case RN_RSI: stream << "si"; break;
        case RN_RDI: stream << "di"; break;
        case RN_RBP: stream << "bp"; break;
        case RN_RSP: stream << "sp"; break;
        case RN_R8: stream << "r8"; break;
        case RN_R9: stream << "r9"; break;
        case RN_R10: stream << "r10"; break;
        case RN_R11: stream << "r11"; break;
        case RN_R12: stream << "r12"; break;
        case RN_R13: stream << "r13"; break;
        case RN_R14: stream << "r14"; break;
        case RN_R15: stream << "r15"; break;
    }
    return stream;
}

typedef struct typed_register {
    RegisterName name;
    OperatorType width;
} TypedRegister;


/** For registers, the width of the instruction matters
 * In general: width + base-name + suffix
 * source: https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf */
inline std::ostream& operator<<(std::ostream& stream, const typed_register& reg) {
    /** Width */
    if (reg.name >= RN_RAX && reg.name <= RN_RSP) {
        if (reg.width == OPT_QUAD || reg.width == OPT_VOID)
            stream << "r";
        else if (reg.width == OPT_DOUBLE)
            stream << "e";
    }

    /** Base */
    stream << reg.name;

    /** Suffix */
    if (reg.name >= RN_RAX && reg.name <= RN_RDX) {
        if (reg.width == OPT_BYTE)
            stream << "l";
        else
            stream << "x";
    } else if (reg.name >= RN_RSI && reg.name <= RN_RSP && reg.width == OPT_BYTE) {
        stream << "l";
    } else if (reg.name >= RN_R8) {
        switch (reg.width) {
            case OPT_BYTE: stream << "b"; break;
            case OPT_WORD: stream << "w"; break;
            case OPT_DOUBLE: stream << "d"; break;
            default: break;
        }
    }
    return stream;
}




#endif //BUILD_REGISTER_H
