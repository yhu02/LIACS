#include "calculator.h"
#include "../../icsyntaxtreevisitor.h"

void ICCalculatorGenerator::visit(const BinaryNode &node) {
    NodeType nt = node.getNodeType();
    switch (nt) {
        case NODE_ADD:
            visit_binary_arithmetics(node, IOP_ADD);
            break;
        case NODE_SUB:
            visit_binary_arithmetics(node, IOP_SUB);
            break;
        case NODE_MUL:
            visit_binary_arithmetics(node, IOP_MUL);
            break;
        default:
            info.logger.error(node.getLineno()) << util::to_string(nt) << " is not a legal BinaryNode\n";
    }
}

void ICCalculatorGenerator::visit(const UnaryNode &node) {
    NodeType nt = node.getNodeType();
    switch (nt) {
        case NODE_SIGNPLUS:
            return node.get_child().accept(parent);
        case NODE_SIGNMINUS: {
            auto temp = std::make_shared<SymbolIOperand>(SymbolIOperand(info.temp_id++));
            node.get_child().accept(parent);
            auto child = info.last_tmp;
            info.ic.emplace_back(node.getReturnType(), IOP_UNARY_MINUS, child, nullptr, temp);
            info.last_tmp = temp;
            break;
        }
        case NODE_PRINT: {
            node.get_child().accept(parent);
            auto child = info.last_tmp;
            info.ic.emplace_back(node.get_child().getReturnType(), IOP_PRINT, child, nullptr, nullptr);
            break;
        }
        default:
            info.logger.error(node.getLineno()) << util::to_string(nt) << " is not a legal UnaryNode\n";
    }
}

void ICCalculatorGenerator::visit(const ConstantNode &node) {
    info.last_tmp = std::make_shared<ImmediateIOperand>(node.get_value());
}