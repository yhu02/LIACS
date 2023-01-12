/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_SYNTAXTREE_H
#define BUILD_SYNTAXTREE_H

#include <memory>
#include "node.h"

/**
 * This file contains a class to represent a SyntaxTree
 * In particular, a SyntaxTree simply contains a single Node, which acts as a root-node
 */

class SyntaxTree {
public:
    explicit SyntaxTree(std::unique_ptr<Node> root) : root(std::move(root)) {}

    [[nodiscard]] const Node& get_root() const { return *root; }

private:
    std::unique_ptr<Node> root;
};

#endif //BUILD_SYNTAXTREE_H
