/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_STACKALLOCATOR_H
#define BUILD_STACKALLOCATOR_H

#include "logger.h"
#include "ioperand.h"
#include "moperand.h"

const size_t STACK_MULTIPLE = 16;


/**
 * Class which defines an abstract StackAllocator
 * a StackAllocator is responsible for converting SymbolIOperands to MOperands containing
 * a memory-reference (stack-offset)
 */
class StackAllocator {
public:
    explicit StackAllocator(Logger& logger) : logger(logger) {}
    virtual ~StackAllocator() = default;

    /**
     * Converts a SymbolIOperand to an MOperand representing a Memory-location on-stack
     * @param sym SymbolIOperand to transform
     * @param sym_width width of the Symbol
     * @param lineno line of the intermediate-code statement
     * @return the generated MOperand
     */
    virtual MOperand to_memory(const SymbolIOperand* sym, size_t sym_width, int lineno) {
        (void)sym;
        (void)sym_width;
        (void)lineno;
        // TODO: Implement in Assignment 5. Make a derived class of StackAllocator and implement there.
        throw std::runtime_error("[StackAllocator::to_memory] Not implemented yet");
    }

    /**
     * @return size_t representing the amount of bytes required for the stack
     */
    [[nodiscard]] virtual size_t get_allocated() const {
        // TODO: Implement in Assignment 5. Make a derived class of StackAllocator and implement there.
        return 0;
    }

protected:
    Logger& logger;
};

#endif //BUILD_STACKALLOCATOR_H
