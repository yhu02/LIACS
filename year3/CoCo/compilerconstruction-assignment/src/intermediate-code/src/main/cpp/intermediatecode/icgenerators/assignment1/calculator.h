#ifndef BUILD_ICCALCULATORGENERATOR_H
#define BUILD_ICCALCULATORGENERATOR_H

#include "../icgenerator.h"
#include "ioperator.h"

class ICCalculatorGenerator : public virtual ICGenerator {
public:
    explicit ICCalculatorGenerator(SyntaxTreeVisitor *parent, SyntaxTreeVisitorInfo &info)
            : ICGenerator(parent, info) {};

protected:
    void visit(const BinaryNode &node) override;

    void visit(const UnaryNode &node) override;

    void visit(const TernaryNode &node) override {
        (void) node;
        throw std::runtime_error("[ICCalculatorGenerator] Cannot process TernaryNode");
    };

    void visit(const ConstantNode &node) override;

    void visit(const SymbolNode &node) override {
        (void) node;
        throw std::runtime_error("[ICCalculatorGenerator] Cannot process SymbolNode");
    };
};

#endif //BUILD_ICCALCULATORGENERATOR_H
