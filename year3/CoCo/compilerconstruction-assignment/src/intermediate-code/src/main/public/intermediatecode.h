/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_INTERMEDIATECODE_H
#define BUILD_INTERMEDIATECODE_H

#include <vector>
#include <logger.h>

#include "istatement.h"
#include "syntaxtree.h"
#include "symboltable.h"

/**
 * File contains a wrapper class for generating IntermediateCode from a SyntaxTree
 * The generated IntermediateCode is iterable
 */

class IntermediateCode {
public:
    IntermediateCode(const SyntaxTree& tree, Logger& logger, SymbolTable& table);

    auto begin() { return ic.begin(); }
    auto end() { return ic.end(); }
    [[nodiscard]] auto cbegin() const { return ic.begin(); }
    [[nodiscard]] auto cend() const { return ic.end(); }
    [[nodiscard]] auto begin() const { return ic.begin(); }
    [[nodiscard]] auto end() const {return ic.end(); }
private:
    std::vector<IStatement> ic;
};

#endif //BUILD_INTERMEDIATECODE_H
