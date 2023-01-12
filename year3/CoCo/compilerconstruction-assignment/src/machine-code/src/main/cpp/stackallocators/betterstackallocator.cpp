#include "betterstackallocator.h"

MOperand BetterStackAllocator::to_memory(const SymbolIOperand *sym, size_t sym_width, int lineno)
{
    (void)lineno;
    size_t offset = variableOffsets[sym->get_id()];
    if (offset != 0)
    {
        return moperand(MOPF_MEM, offset);
    }

    allocated += sym_width;
    variableOffsets[sym->get_id()] = allocated;
    return moperand(MOPF_MEM, allocated);
}

size_t BetterStackAllocator::get_allocated() const
{
    int m = 16;
    if ((allocated % m) == 0)
    {
        return allocated;
    }
    return ((allocated / m) + 1) * m;
}