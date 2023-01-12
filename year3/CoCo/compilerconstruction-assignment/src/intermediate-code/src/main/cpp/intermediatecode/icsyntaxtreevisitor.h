#ifndef BUILD_ICSYNTAXTREEVISITOR_H
#define BUILD_ICSYNTAXTREEVISITOR_H

#include <memory>

#include "node.h"
#include "logger.h"
#include "symboltable.h"
#include "./icgenerators/assignment1/calculator.h"
#include "./icgenerators/assignment2/conditionals.h"
#include "./icgenerators/assignment3/types.h"
#include "./icgenerators/assignment4/divmod.h"
#include "ioperand.h"
#include "istatement.h"

/**
 * This file contains the entry-point for transforming a SyntaxTree to Intermediate code
 */


/**
 * Info required by all visitors
 */
struct SyntaxTreeVisitorInfo {
    SyntaxTreeVisitorInfo(Logger& logger, SymbolTable& table): logger(logger), table(table) {}

    std::shared_ptr<IOperand> last_tmp; // Use when you need to assign the same IOperand to multiple IStatements
    std::vector<IStatement> ic = {}; // generated IStatements
    Logger& logger;
    SymbolTable& table;
    size_t temp_id = table.getNumberSymbols();
    // Keep track of which label we should use
    size_t label_id = 0;
};

/**
 * Entry-point for transforming a SyntaxTree into Intermediate-code.
 * This class takes care to propagate each node to the right assignment-visitor.
 * E.g., if-statements will be processed by assignment 2
 *
 * Each assignment-specific visitor may use this visitor as 'parent' to process generic Nodes.
 */
class SyntaxTreeVisitor : public AbstractSyntaxTreeVisitor {
public:
    explicit SyntaxTreeVisitor(Logger& logger, SymbolTable& table) : info(logger, table) {
        this->ass1 = std::make_unique<ICCalculatorGenerator>(this, info);
        this->ass2 = std::make_unique<ICConditionalGenerator>(this, info);
        this->ass3 = std::make_unique<ICTypeGenerator>(this, info);
        this->ass4 = std::make_unique<ICDivModGenerator>(this, info);
    };

    virtual void accept(const Node& node) { node.accept(this); };
    virtual std::vector<IStatement> retrieve_ic() { return std::move(info.ic); };

    void visit(const BinaryNode& node) override {
        switch (node.getAssignmentType()) {
            case ASS_FIVE: break;
            case ASS_FOUR: 
                if (this->ass4 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit BINARY] `ass4` is not set");
                ass4->visit(node); break;
            case ASS_THREE: 
                if (this->ass3 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit BINARY] `ass3` is not set");
                ass3->visit(node); break;
            case ASS_TWO: 
                if (this->ass2 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit BINARY] `ass2` is not set");
                ass2->visit(node); break;
            case ASS_ONE: 
                if (this->ass1 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit BINARY] `ass1` is not set");
                ass1->visit(node); break;
        }
    };
    void visit(const UnaryNode& node) override {
        switch (node.getAssignmentType()) {
            case ASS_FIVE: break;
            case ASS_FOUR: 
                if (this->ass4 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit UNARY] `ass4` is not set");
                ass4->visit(node); break;
            case ASS_THREE: 
                if (this->ass3 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit UNARY] `ass3` is not set");
                ass3->visit(node); break;
            case ASS_TWO: 
                if (this->ass2 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit UNARY] `ass2` is not set");
                ass2->visit(node); break;
            case ASS_ONE: 
                if (this->ass1 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit UNARY] `ass1` is not set");
                ass1->visit(node); break;
        }
    };
    void visit(const TernaryNode& node) override {
        switch (node.getAssignmentType()) {
            case ASS_FIVE: break;
            case ASS_FOUR: 
                if (this->ass4 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit TERNARY] `ass4` is not set");
                ass4->visit(node); break;
            case ASS_THREE: 
                if (this->ass3 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit TERNARY] `ass3` is not set");
                ass3->visit(node); break;
            case ASS_TWO: 
                if (this->ass2 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit TERNARY] `ass2` is not set");
                ass2->visit(node); break;
            case ASS_ONE: 
                if (this->ass1 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit TERNARY] `ass1` is not set");
                ass1->visit(node); break;
        }
    };
    void visit(const ConstantNode& node) override {
        switch (node.getAssignmentType()) {
            case ASS_FIVE: break;
            case ASS_FOUR: 
                if (this->ass4 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit CONSTANT] `ass4` is not set");
                ass4->visit(node); break;
            case ASS_THREE: 
                if (this->ass3 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit CONSTANT] `ass3` is not set");
                ass3->visit(node); break;
            case ASS_TWO: 
                if (this->ass2 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit CONSTANT] `ass2` is not set");
                ass2->visit(node); break;
            case ASS_ONE: 
                if (this->ass1 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit CONSTANT] `ass1` is not set");
                ass1->visit(node); break;
        }
    };
    void visit(const SymbolNode& node) override {
        switch (node.getAssignmentType()) {
            case ASS_FIVE: break;
            case ASS_FOUR: 
                if (this->ass4 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit SYMBOL] `ass4` is not set");
                ass4->visit(node); break;
            case ASS_THREE: 
                if (this->ass3 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit SYMBOL] `ass3` is not set");
                ass3->visit(node); break;
            case ASS_TWO: 
                if (this->ass2 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit SYMBOL] `ass2` is not set");
                ass2->visit(node); break;
            case ASS_ONE: 
                if (this->ass1 == nullptr)
                    throw std::runtime_error("[SyntaxTreeVisitor::visit SYMBOL] `ass1` is not set");
                ass1->visit(node); break;
        }
    };
    // we assume we never need to change it
    void visit(const EmptyNode& node) override { (void)node; };

protected:
    SyntaxTreeVisitorInfo info;
    std::unique_ptr<AbstractSyntaxTreeVisitor> ass1 = nullptr;
    std::unique_ptr<AbstractSyntaxTreeVisitor> ass2 = nullptr;
    std::unique_ptr<AbstractSyntaxTreeVisitor> ass3 = nullptr;
    std::unique_ptr<AbstractSyntaxTreeVisitor> ass4 = nullptr;
};

#endif //BUILD_ICSYNTAXTREEVISITOR_H
