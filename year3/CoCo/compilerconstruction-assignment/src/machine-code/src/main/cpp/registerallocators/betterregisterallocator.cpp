#include "betterregisterallocator.h"

MOperand BetterRegisterAllocator::to_register(const SymbolIOperand *sym, int lineno) const {
    (void) sym;
    (void) lineno;
    throw std::runtime_error("[BetterRegisterAllocater::to_register] Not implemented yet");
}