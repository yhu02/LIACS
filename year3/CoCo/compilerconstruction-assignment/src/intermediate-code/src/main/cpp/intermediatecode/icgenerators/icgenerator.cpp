#include "icgenerator.h"
#include "../icsyntaxtreevisitor.h"

void ICGenerator::visit_binary_arithmetics(const BinaryNode& node, IOperator iop) {
    node.get_left_child().accept(parent);
    auto left = info.last_tmp;
    node.get_right_child().accept(parent);
    auto right = info.last_tmp;
    auto temp = std::make_shared<SymbolIOperand>(SymbolIOperand(info.temp_id++));
    info.ic.emplace_back(node.getReturnType(), iop, left, right, temp);
    info.last_tmp = temp;
}

void ICGenerator::visit_not_condition(const BinaryNode &condition, IOperator iop) {
    // TODO: Implement in Assignment 2
    condition.get_left_child().accept(parent);
    auto child = info.last_tmp;

    condition.get_right_child().accept(parent);
    auto child2 = info.last_tmp;
    auto label = std::make_shared<SymbolIOperand>(info.label_id++);

    info.last_tmp = label;

    if(child2->getOperandType() == OT_SYMBOL) { 
        auto child_sym = std::make_shared<SymbolIOperand>(info.label_id++);

        info.ic.emplace_back(condition.getReturnType(), IOP_ASSIGN, child, nullptr, child_sym);
        info.ic.emplace_back(condition.getReturnType(), iop, child2, child_sym, label);
    }else{
        info.ic.emplace_back(condition.getReturnType(), iop, child2, child, label);
    }
    /**
     * input: OP1 !IOP_COND OP2
     *
     * Note: dst is compared with src
     * AT&T syntax: op src dst
     * E.g. 2 < 3, should then be op 3, reg, where reg contains value 2
     *
     * output:  !IOP_COND OP2 OP1 LABEL1
     * or:
     *          ASSIGN OP1 OP1_SYM
     *          !IOP_COND OP2 OP1_SYM LABEL1
     */
}