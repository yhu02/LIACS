/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_SYMBOLTABLE_H
#define BUILD_SYMBOLTABLE_H

#include <vector>
#include "symbol.h"

/**
 * SymbolTable containing Symbols
 */
class SymbolTable {
public:
    SymbolTable() = default;
    ~SymbolTable() = default;

    /**
     * Adds symbols to the back of symbols by using a forward_iterator
     * @tparam It an Iterator type with the forward_iterator_tag
     * @param begin start of the iterator
     * @param end end of the iterator
     * @return first index
     */
    template<typename It = typename std::forward_iterator_tag>
    size_t addSymbols(It begin, It end) { size_t id = symbols.size(); std::move(begin, end, std::back_inserter(symbols)); return id; }

    /**
     * Returns the symbol at the given index
     * @param index size_t of index
     * @return a reference to a std::unique_ptr to the Symbol, or nullptr
     */
    std::unique_ptr<Symbol>* getSymbol(size_t index) {
        if (index >= symbols.size())
            return nullptr;
        return &symbols[index];
    }

    /**
     * @return number of symbols in the table
     */
    [[nodiscard]] size_t getNumberSymbols() const { return symbols.size(); }

private:
    std::vector<std::unique_ptr<Symbol>> symbols = {};
};

#endif //BUILD_SYMBOLTABLE_H
