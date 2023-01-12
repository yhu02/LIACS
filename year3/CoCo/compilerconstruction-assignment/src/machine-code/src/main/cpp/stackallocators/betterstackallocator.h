#ifndef BETTERSTACKALLOCATOR_H
#define BETTERSTACKALLOCATOR_H

#include "./stackallocator.h"
#include <map>

class BetterStackAllocator : public StackAllocator {
public:
    explicit BetterStackAllocator(Logger &logger) : StackAllocator(logger) {}

    virtual ~BetterStackAllocator() = default;

    MOperand to_memory(const SymbolIOperand *sym, size_t sym_width, int lineno);

    [[nodiscard]] size_t get_allocated() const;
private:
    size_t allocated = 0;

    std::map<size_t, size_t> variableOffsets; // <id, offset>
};

#endif //BETTERSTACKALLOCATOR_H
