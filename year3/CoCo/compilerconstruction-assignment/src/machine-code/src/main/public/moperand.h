/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_MOPERAND_H
#define BUILD_MOPERAND_H

#include <string>
#include <limits>

#include "register.h"

/**
 * This class contains several utilities to represent an MachineCode-Operand
 * Moperand consists of:
 *      - MOperand_flag: indicates the type and validity
 *      - MOperand_mem: contains the actual value in memory; either a RegisterName or an uint8_t
 * Also contains printing and predefined-MOperand functionalities
 */

typedef union moperand_mem {
    RegisterName registerName;
    uint64_t value;
    off_t offset;
} MOperand_mem;

typedef enum moperand_flag {
    MOPF_REG,
    MOPF_VAL,
    MOPF_MEM,
    MOPF_NONE,
    MOPF_ERROR
} MOperand_flag;

typedef struct moperand {
    MOperand_flag flag;
    MOperand_mem value;

    moperand(MOperand_flag flag, uint64_t num) : flag(flag) { value.value = num; }
    moperand(MOperand_flag flag, RegisterName name) : flag(flag) { value.registerName = name; }
    moperand(MOperand_flag flag, off_t offset) : flag(flag) { value.offset = offset; }
    moperand(MOperand_flag flag, MOperand_mem value)  : flag(flag), value(value) {}
} MOperand;

typedef struct typed_moperand {
    MOperand mop;
    OperatorType width;
} TypedMOperand;

inline MOperand error_moperand() { return {MOPF_ERROR, std::numeric_limits<uint64_t>::max()}; }
inline MOperand empty_moperand() { return {MOPF_NONE, uint64_t(0)}; }
inline std::ostream& operator<<(std::ostream& stream, const typed_moperand& tmop) {
    switch (tmop.mop.flag) {
        case MOPF_VAL: stream << "$" << int32_t(tmop.mop.value.value); break;
        case MOPF_MEM: stream << "-" << tmop.mop.value.offset << "(%rbp)"; break;
        case MOPF_NONE: break;
        case MOPF_ERROR: stream << "err"; break;
        case MOPF_REG: {
            TypedRegister reg = {tmop.mop.value.registerName, tmop.width};
            stream << "%" << reg;
            break;
        }
    }
    return stream;
}

#endif //BUILD_MOPERAND_H
