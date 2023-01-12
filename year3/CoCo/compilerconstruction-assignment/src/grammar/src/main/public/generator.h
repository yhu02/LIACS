/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_GENERATOR_H
#define BUILD_GENERATOR_H

#include <string>
#include <logger.h>
#include "syntaxtree.h"
#include "symboltable.h"


namespace grammar::generator {
    /**
     * Generates a SyntaxTree from a FILE, if no errors occur
     * @param file The file to parse
     * @param logger Logger to log messages to
     * @param tree A pointer to the SyntaxTree to assign
     * @return zero on success, non-zero on failure
     */
    int generate(FILE* file, Logger& logger, SymbolTable& table, std::unique_ptr<SyntaxTree>& tree);
    /**
     * Generates a SyntaxTree from a string-path to a valid file, if no errors occur
     * @param filename string of the path to a valid file
     * @param logger Logger to log messages to
     * @param tree A pointer to the SyntaxTree to assign
     * @return zero on success, non-zero on failure
     */
    int generate(const std::string& filename, Logger& logger, SymbolTable& table, std::unique_ptr<SyntaxTree>& tree);
}

#endif //BUILD_GENERATOR_H
