#include "conditionals.h"
#include "../../icsyntaxtreevisitor.h"

void ICConditionalGenerator::visit(const BinaryNode& node) {
    NodeType nt = node.getNodeType();
    switch (nt) {
        case NODE_EQ: visit_not_condition(node, IOP_JNE); break;
        case NODE_NEQ: visit_not_condition(node, IOP_JE); break;
        case NODE_GEQ: visit_not_condition(node, IOP_JL); break;
        case NODE_GT: visit_not_condition(node, IOP_JLE); break;
        case NODE_LEQ: visit_not_condition(node, IOP_JG); break;
        case NODE_LT: visit_not_condition(node, IOP_JGE); break;
        case NODE_IF: visit_if_statement(node); break;
        default: info.logger.error(node.getLineno()) << util::to_string(nt) << " is not a legal BinaryNode\n";
    }
}

void ICConditionalGenerator::visit(const TernaryNode &node) {
    // TODO: Implement in Assignment 2
    NodeType nt = node.getNodeType();
    
    switch (nt) {
        case NODE_IF_ELSE: visit_if_else_statement(node); break;
        default: info.logger.error(node.getLineno()) << util::to_string(nt) << " is not a legal TernaryNode\n";
    }
}

void ICConditionalGenerator::visit(const ConstantNode& node) {
    info.last_tmp = std::make_shared<ImmediateIOperand>(node.get_value());
}

void ICConditionalGenerator::visit_if_statement(const BinaryNode& node) {
    // TODO: Implement in Assignment 2
    NodeType nt = node.getNodeType();
    if (nt == NODE_IF) {
        node.get_left_child().accept(parent);
        auto child = info.last_tmp;

        node.get_right_child().accept(parent);
        auto child2 = info.last_tmp;
        
        info.ic.emplace_back(node.getReturnType(), IOP_LABEL, child, nullptr, nullptr);
    }
    /**
     * input: IF COND THEN STMT1
     *
     * output:
     * !IOP_COND OP1 OP2 LABEL
     * STMT1
     * LABEL
     */

    
}

void ICConditionalGenerator::visit_if_else_statement(const TernaryNode& node) {
    // TODO: Implement in Assignment 2
    NodeType nt = node.getNodeType();

    if (nt == NODE_IF_ELSE) {
        node.get_left_child().accept(parent);
        auto child = info.last_tmp;
    
        node.get_middle_child().accept(parent);
        auto child2 = info.last_tmp;
    
        auto label2 = std::make_shared<SymbolIOperand>(info.label_id++);

        info.ic.emplace_back(node.getReturnType(), IOP_GOTO, label2, label2, label2);
        info.ic.emplace_back(node.getReturnType(), IOP_LABEL, child, nullptr, nullptr);

        node.get_right_child().accept(parent);
        auto child3 = info.last_tmp;

        info.ic.emplace_back(node.getReturnType(), IOP_LABEL, label2, nullptr, nullptr);
    }
    
    /**
     * input: IF COND THEN STMT1 ELSE STMT2
     *
     * output:
     * !IOP_COND OP1 OP2 LABEL1
     * STMT1
     * GOTO LABEL2
     * LABEL1
     * STMT2
     * LABEL2
     */
}







