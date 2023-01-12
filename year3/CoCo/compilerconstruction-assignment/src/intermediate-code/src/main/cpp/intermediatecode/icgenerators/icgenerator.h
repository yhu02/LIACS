#ifndef BUILD_ICGENERATOR_H
#define BUILD_ICGENERATOR_H

#include "syntaxtreevisitor.h"
#include "ioperator.h"

class SyntaxTreeVisitor;

struct SyntaxTreeVisitorInfo;

/**
 * Generic Intermediate-Code Generator through the AbstractSyntaxTreeVisitor interface
 * Contains a SyntaxTreeVisitor 'parent' for the generic visit functions.
 * E.g. if we currently process an IF-statement containing NODE_ASSIGNMENT in its body, then
 *      the IF-statement could be processed by one visitor, and its body by another using
 *      'node.get_right_child().accept(parent)'
 *
 * Through this generator one can add functions which may be used by visitors from multiple assignments.
 * E.g., a condition may be needed both by assignment 2 and assignment 4.
 */
class ICGenerator : public AbstractSyntaxTreeVisitor {
public:
    ICGenerator(SyntaxTreeVisitor *parent, SyntaxTreeVisitorInfo &info) : parent(parent),
                                                                          info(info) {}
    void visit(const UnaryNode &node) override = 0;

    void visit(const BinaryNode &node) override = 0;

    void visit(const TernaryNode &node) override = 0;

    void visit(const ConstantNode &node) override = 0;

    void visit(const SymbolNode &node) override = 0;

    // we assume we never need to change it
    void visit(const EmptyNode &node) override { (void) node; };

protected:
    SyntaxTreeVisitor *parent;
    SyntaxTreeVisitorInfo &info;

    void visit_binary_arithmetics(const BinaryNode &node, IOperator iop);

    void visit_not_condition(const BinaryNode &condition, IOperator iop);
};

#endif //BUILD_ICGENERATOR_H
