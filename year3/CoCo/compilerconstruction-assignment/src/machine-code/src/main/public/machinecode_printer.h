/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_MACHINECODE_PRINTER_H
#define BUILD_MACHINECODE_PRINTER_H

#include "machinecode.h"

#include <ostream>

/**
 * This file contains several functions to generate machinecode from an inputfile
 * In particular, it executes all major compilation stages (parsing, ic-generation, mc-generation) and
 * generates x86-64 assembly afterwards
 */

namespace machinecode::generator {
        /**
         * Streams the main function of the provided MachineCode
         * Prologue and Epilogue are auto-generated
         * @param stream ostream to stream to
         * @param mc generated MachineCode containing the instructions to stream
         */
        void print_main(std::ostream& stream, MachineCode& mc);
        /**
         * Streams builtin functions
         * Currently supported:
         *      - writeinteger: prints the integer saved in register RDI
         * @param stream ostream to stream to
         */
        void print_builtins(std::ostream& stream);

        /**
         * Executes all major compilation stages to generate x86-64 assembly
         * @param logger Logger to log messages to
         * @param inputFilePath string of path containing the input
         * @param outputFilePath string of path where output should be printed to
         * @return 0 on success, otherwise an appropriate ErrorCode
         */
        int generate(Logger& logger, SymbolTable& table, const std::string& inputFilePath, const std::string& outputFilePath="");
        /**
         * Executes all major compilation stages to generate x86-64 assembly
         * @param logger Logger to log messages to
         * @param inputFilePath string of path containing the input
         * @param stream ostream to stream output to
         * @return 0 on success, otherwise an appropriate ErrorCode
         */
        int generate(Logger& logger, SymbolTable& table, const std::string& inputFilePath, std::ostream& stream);
    }

#endif //BUILD_MACHINECODE_PRINTER_H
