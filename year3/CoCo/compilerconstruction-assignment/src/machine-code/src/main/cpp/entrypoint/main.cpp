/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include <iostream>
#include <tclap/CmdLine.h>

#include "machinecode_printer.h"
#include "generator.h"

int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("CoCo Compiler - Machine Code Generator", ' ', "1.0");

    try {
        TCLAP::ValueArg<std::string> input_file_path_arg("f", "file", "Path to source file.", true, "", "string");
        TCLAP::ValueArg<std::string> output_file_path_arg("o", "output", "Output file to write generated assembly to, if not set, outputs to stdout", false, "", "string");
        TCLAP::SwitchArg output_switch("n", "no-output", "Does not generate any output", false);

        TCLAP::EitherOf outputs;
        outputs.add(output_file_path_arg).add(output_switch);
        cmd.add(outputs);
        cmd.add(input_file_path_arg);
        cmd.parse(argc, argv);

        Logger logger(std::cout, std::cout, std::cout);
        SymbolTable table;
        if (output_switch.isSet())
            return machinecode::generator::generate(logger, table, input_file_path_arg.getValue(), NULL_STREAM);
        else if (output_file_path_arg.isSet())
            return machinecode::generator::generate(logger, table, input_file_path_arg.getValue(), output_file_path_arg.getValue());
        else
            return machinecode::generator::generate(logger, table, input_file_path_arg.getValue(), std::cout);

    } catch (TCLAP::ArgException& e) {
        std::cerr << "command-line error: " << e.error()
                  << " for arg " << e.argId() << std::endl;
        return 1;
    }
}