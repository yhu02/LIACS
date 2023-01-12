#ifndef BUILD_ASS4GENERATOR_H
#define BUILD_ASS4GENERATOR_H

#include "src/main/cpp/intermediatecode/icgenerators/assignment1/calculator.h"
#include "src/main/cpp/intermediatecode/icgenerators/assignment2/conditionals.h"

class ICDivModGenerator: public virtual ICGenerator {
public:
    explicit ICDivModGenerator(SyntaxTreeVisitor* parent, SyntaxTreeVisitorInfo& info) : ICGenerator(parent, info) {}

protected:
    void visit(const BinaryNode& node) override;
    void visit(const UnaryNode& node) override {
        (void)node;
        throw std::runtime_error("[ASS4Visitor] Cannot process UnaryNode");
    };
    void visit(const TernaryNode& node) override {
        (void)node;
        throw std::runtime_error("[ASS4Visitor] Cannot process TernaryNode");
    };
    void visit(const ConstantNode& node) override {
        (void)node;
        throw std::runtime_error("[ASS4Visitor] Cannot process ConstantNode");
    };
    void visit(const SymbolNode& node) override {
        (void)node;
        throw std::runtime_error("[ASS4Visitor] Cannot process SymbolNode");
    };

    void visit_while_statement(const BinaryNode& node);
    void visit_div_statement(const BinaryNode& node, IOperator iop);
};

#endif //BUILD_ASS4GENERATOR_H
