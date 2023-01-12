#ifndef BUILD_TYPEGENERATOR_H
#define BUILD_TYPEGENERATOR_H

#include "../icgenerator.h"

class ICTypeGenerator : public virtual ICGenerator {
public:
    explicit ICTypeGenerator(SyntaxTreeVisitor* parent, SyntaxTreeVisitorInfo& info) : ICGenerator(parent, info) {}

protected:
    void visit(const BinaryNode& node) override;
    void visit(const UnaryNode& node) override {
        (void)node;
        throw std::runtime_error("[ICTypeGenerator] Cannot process UnaryNode");
    }
    void visit(const TernaryNode& node) override {
        (void)node;
        throw std::runtime_error("[ICTypeGenerator] Cannot process TernaryNode");
    }
    void visit(const ConstantNode& node) override {
        (void)node;
        throw std::runtime_error("[ICTypeGenerator] Cannot process ConstantNode");
    }
    void visit(const SymbolNode& node) override;
};

#endif //BUILD_TYPEGENERATOR_H
