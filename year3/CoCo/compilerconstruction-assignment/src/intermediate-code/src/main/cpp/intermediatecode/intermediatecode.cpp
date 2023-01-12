/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "intermediatecode.h"
#include "icsyntaxtreevisitor.h"

IntermediateCode::IntermediateCode(const SyntaxTree& tree, Logger& logger, SymbolTable& table) {
    SyntaxTreeVisitor vis(logger, table);
    vis.accept(tree.get_root());
    ic = vis.retrieve_ic();
}
