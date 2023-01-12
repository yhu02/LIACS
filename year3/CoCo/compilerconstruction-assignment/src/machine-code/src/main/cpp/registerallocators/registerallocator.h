/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_REGISTERALLOCATOR_H
#define BUILD_REGISTERALLOCATOR_H

#include <ioperand.h>
#include <moperand.h>
#include <unordered_map>
#include <algorithm>
#include "logger.h"

/**
 * Abstract class to define a RegisterAllocator
 * A RegisterAllocator is used to convert a SymbolIOperand to a MOperand containing a register
 */

class RegisterAllocator {
public:
    explicit RegisterAllocator(Logger& logger) : logger(logger) {}
    virtual ~RegisterAllocator() = default;

    /**
     * Converts a SymbolIOperand to a MOperand
     * Fails if the SymbolIOperand is a nullptr, or if there are no registers left
     * @param sym the SymbolIOperand to convert
     * @return the converted MOperand, or an error_moperand() on error
     */
    virtual MOperand to_register(const SymbolIOperand* sym, int lineno) const {
        // TODO: Better implementation in Assignment 5. Make a derived class of RegisterAllocator and implement there. 
        (void)lineno;
        static std::vector<RegisterName> safe_registers = {RN_RAX, RN_RBX, RN_RCX, RN_RDX, RN_RSI, RN_R8, RN_R9, RN_R10, RN_R11, RN_R12, RN_R13, RN_R14, RN_R15};
        static std::unordered_map<size_t, RegisterName> registers;

        static bool once = true;

        if (once) {
            if (CURRENT_ASSIGNMENT & ASS_FOUR) {
                safe_registers.erase(std::remove(safe_registers.begin(), safe_registers.end(), RN_RDX), safe_registers.end());
            }
            
            if (CURRENT_ASSIGNMENT & ASS_THREE) {
                safe_registers.erase(std::remove(safe_registers.begin(), safe_registers.end(), RN_RAX), safe_registers.end());
            }
            
            once = false;
        }

        if (registers.find(sym->get_id()) == registers.end()) {
            if (safe_registers.size() == 0) {
                throw std::runtime_error("No more registers left");
            }
            registers[sym->get_id()] = *safe_registers.begin();
            safe_registers.erase(safe_registers.begin());
        }
        return {MOPF_REG, registers[sym->get_id()]};
    }

protected:
    Logger& logger;
};

#endif //BUILD_REGISTERALLOCATOR_H
