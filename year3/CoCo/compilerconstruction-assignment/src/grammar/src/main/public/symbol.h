/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_SYMBOL_H
#define BUILD_SYMBOL_H

#include <string>
#include <utility>
#include "types.h"

/**
 * Symbol class for elements in the SymbolTable
 * Each symbol contains an std::string for its name, and a ReturnType for its type
 * Additionally we keep track of an integer for the line on which it was declared
 */
class Symbol {
public:
    Symbol(std::string name, int line, ReturnType returnType) : name(std::move(name)), line(line), returnType(returnType) {}

    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] int getLine() const { return line; }
    [[nodiscard]] ReturnType getReturnType() const { return returnType; }

private:
    std::string name;
    int line = -1;
    ReturnType returnType = RT_UNKNOWN;
};

#endif //BUILD_SYMBOL_H
