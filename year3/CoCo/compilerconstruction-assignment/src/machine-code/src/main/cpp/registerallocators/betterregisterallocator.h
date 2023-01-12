#ifndef BETTERREGISTERALLOCATOR_H
#define BETTERREGISTERALLOCATOR_H

#include "./registerallocator.h"

class BetterRegisterAllocator : public RegisterAllocator {
public:
    explicit BetterRegisterAllocator(Logger &logger) : RegisterAllocator(logger) {}

    virtual ~BetterRegisterAllocator() = default;

    MOperand to_register(const SymbolIOperand *sym, int lineno) const;
};

#endif //BETTERREGISTERALLOCATOR_H
