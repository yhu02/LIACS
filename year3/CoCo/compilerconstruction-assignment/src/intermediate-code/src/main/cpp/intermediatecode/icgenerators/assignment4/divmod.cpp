#include "divmod.h"
#include "../../icsyntaxtreevisitor.h"

void ICDivModGenerator::visit(const BinaryNode &node) {
    NodeType nt = node.getNodeType();
    switch (nt) {
        case NODE_WHILE:
            visit_while_statement(node);
            break;
        case NODE_DIV:
            visit_div_statement(node, IOP_DIV);
            break;
        case NODE_MOD:
            visit_div_statement(node, IOP_MOD);
            break;
        default:
            info.logger.error(node.getLineno()) << util::to_string(nt) << " is not a legal BinaryNode\n";
    }
}

void ICDivModGenerator::visit_while_statement(const BinaryNode &node) {
    // TODO: Implement in Assignment 4
    NodeType nt = node.getNodeType();
    if (nt == NODE_WHILE) {
        auto label1 = std::make_shared<SymbolIOperand>(info.label_id++);
        info.ic.emplace_back(node.getReturnType(), IOP_LABEL, label1, nullptr, nullptr);

        node.get_left_child().accept(parent);
        auto child = info.last_tmp;
        node.get_right_child().accept(parent);

        info.ic.emplace_back(node.getReturnType(), IOP_GOTO, label1, label1, label1);
        info.ic.emplace_back(node.getReturnType(), IOP_LABEL, child, nullptr, nullptr);
    }

    /**
     * input: WHILE COND STMT
     *
     * output:
     * LABEL1
     * !IOP_COND LABEL2
     * STMT
     * GOTO LABEL1
     * LABEL2
     */
}

void ICDivModGenerator::visit_div_statement(const BinaryNode &node, IOperator iop) {
    // TODO: Implement in Assignment
    node.get_left_child().accept(parent);
    auto lChild = info.last_tmp;

    node.get_right_child().accept(parent);
    auto rChild = info.last_tmp;

    if (node.get_right_child().getNodeType() != NODE_ID) {
        auto res = std::make_shared<SymbolIOperand>(info.temp_id++);
        info.ic.emplace_back(node.get_right_child().getReturnType(), IOP_ASSIGN, rChild, nullptr, res);
        info.last_tmp = std::make_shared<SymbolIOperand>(info.temp_id++);
        info.ic.emplace_back(node.get_left_child().getReturnType(), iop, lChild, res, info.last_tmp);
    } else {
        info.last_tmp = std::make_shared<SymbolIOperand>(info.temp_id++);
        info.ic.emplace_back(node.get_left_child().getReturnType(), iop, lChild, rChild, info.last_tmp);
    }

    /**
     * Similar to visit_binary_arithmetic, but with the condition that OP2 should be a symbol
     *
     * output:
     *      IOP OP1 OP2
     * or:
     *      ASSIGN OP2 OP2_sym
     *      IOP OP1 OP2_sym
     */
}

