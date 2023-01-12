#ifndef BUILD_ICIFELSEGENERATOR_H
#define BUILD_ICIFELSEGENERATOR_H

#include <src/main/cpp/intermediatecode/icgenerators/icgenerator.h>
#include "ioperator.h"

class ICConditionalGenerator : public virtual ICGenerator {
public:
    explicit ICConditionalGenerator(SyntaxTreeVisitor* parent, SyntaxTreeVisitorInfo& info) : ICGenerator(parent, info) {}

protected:
    void visit(const BinaryNode& node) override;
    void visit(const UnaryNode& node) override {
        (void)node;
        throw std::runtime_error("[ICConditionalGenerator] Cannot process UnaryNode");
    }
    void visit(const TernaryNode& node) override;
    void visit(const ConstantNode& node) override;
    void visit(const SymbolNode& node) override {
        (void)node;
        throw std::runtime_error("[ICConditionalGenerator] Cannot process SymbolNode");
    };


    void visit_if_statement(const BinaryNode& node);
    void visit_if_else_statement(const TernaryNode& node);
};

#endif //BUILD_ICIFELSEGENERATOR_H
