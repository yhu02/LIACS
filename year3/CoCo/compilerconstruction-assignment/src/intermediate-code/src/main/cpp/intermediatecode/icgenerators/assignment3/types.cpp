#include "types.h"
#include "../../icsyntaxtreevisitor.h"

void ICTypeGenerator::visit(const BinaryNode &node) {
    // TODO: Implement in Assignment 3
    if (node.getNodeType() == NODE_LIST) {
        node.get_left_child().accept(parent);
        node.get_right_child().accept(parent);
        return;
    }

    if (node.getNodeType() == NODE_ASSIGNMENT) {
        node.get_left_child().accept(parent);
        auto targetVariable = info.last_tmp;
        node.get_right_child().accept(parent);
        auto value = info.last_tmp;
        info.ic.emplace_back(node.get_left_child().getReturnType(), IOP_ASSIGN, value, nullptr, targetVariable);
        return;
    }

    throw std::runtime_error("[ICTypeGenerator::visit BINARY] Invalid node type " + std::to_string(node.getNodeType()));
}

void ICTypeGenerator::visit(const SymbolNode &node) {
    auto symbol = std::make_shared<SymbolIOperand>(node.get_id());
    info.last_tmp = symbol;
}