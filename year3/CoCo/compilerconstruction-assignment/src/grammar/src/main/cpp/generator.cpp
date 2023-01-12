/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "generator.h"
#include <iostream>
#include <src/main/cpp/grammarbuilder//grammarbuilder.h>

int yyparse(GrammarBuilder&);
int yyset_lineno(int);
int yylex_destroy();
extern FILE* yyin;

extern int lineno;

int grammar::generator::generate(FILE* file, Logger& logger, SymbolTable& table, std::unique_ptr<SyntaxTree>& tree) {
    // Explicitly sets lineno to 1 (first line in lex). Subsequent calls start counting lines from 1 this way.
    yyset_lineno(1);
    if (file)
        yyin = file;

    GrammarBuilder builder(logger, table);
    int parsed = yyparse(builder);
    if (logger.n_errors() || parsed != 0)
        return parsed;
    tree = builder.retrieve_syntaxtree();
    yylex_destroy();
    return parsed;
}

int grammar::generator::generate(const std::string& filename, Logger& logger,  SymbolTable& table, std::unique_ptr<SyntaxTree>& tree) {
    FILE* file = std::fopen(filename.c_str(), "r");
    if (!file) {
        std::cerr << "Could not open " << filename << std::endl;
        return -1;
    }
    int parsed = generate(file, logger, table, tree);
    fclose(file);
    return parsed;
}
