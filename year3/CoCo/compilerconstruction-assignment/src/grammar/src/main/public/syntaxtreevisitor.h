/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_SYNTAXTREEVISITOR_H
#define BUILD_SYNTAXTREEVISITOR_H

#include <memory>
#include <iostream>

/**
 * This file contains an Abstract class for a SyntaxTreeVisitor.
 * This may be used by later modules to implement a Visitor for the SyntaxTree
 */

class UnaryNode;
class BinaryNode;
class TernaryNode;
class ConstantNode;
class SymbolNode;
class EmptyNode;

class AbstractSyntaxTreeVisitor {
public:
    virtual ~AbstractSyntaxTreeVisitor() = default;

    virtual void visit(const UnaryNode& node) = 0;
    virtual void visit(const BinaryNode& node) = 0;
    virtual void visit(const TernaryNode& node) = 0;
    virtual void visit(const ConstantNode& node) = 0;
    virtual void visit(const SymbolNode& node) = 0;
    virtual void visit(const EmptyNode& node) = 0;
};

#endif //BUILD_SYNTAXTREEVISITOR_H
