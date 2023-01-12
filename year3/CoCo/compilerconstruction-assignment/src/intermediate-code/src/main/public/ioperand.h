/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_IOPERAND_H
#define BUILD_IOPERAND_H

#include <memory>
#include <limits>

/**
 * File contains classes and Enums to represent an IOperand
 * IOperands are used to represent the operands in an IStatement
 */

typedef enum ioperandtype {
    OT_UNKNOWN, // Unknown (yet)
    OT_IMM,     // Immediate
    OT_SYMBOL   // Symbol object
} IOperandType;

class IOperand {
public:
    virtual ~IOperand() = default;
    explicit IOperand(IOperandType iOperandType) : iOperandType(iOperandType) {}

    [[nodiscard]] IOperandType getOperandType() const { return iOperandType; }

private:
    IOperandType iOperandType = OT_UNKNOWN;
};

class ImmediateIOperand : public IOperand {
public:
    explicit ImmediateIOperand(uint64_t value) : IOperand(OT_IMM), value(value) {}

    [[nodiscard]] uint64_t get_value() const { return value; }
private:
    uint64_t value;
};

class SymbolIOperand : public IOperand {
public:
    explicit SymbolIOperand(size_t id) : IOperand(OT_SYMBOL), id(id) {}

    [[nodiscard]] size_t get_id() const { return id; }
private:
    size_t id = std::numeric_limits<size_t>::max();
};

#endif //BUILD_IOPERAND_H
