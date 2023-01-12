#include "types.h"
#include "../../intermediatecodevisitor.h"

void MCTypesGenerator::accept(const IStatement &stmt) {
    // TODO: Implement in Assignment 3

    if (stmt.get_ioperator() != IOP_MUL) {
        throw std::runtime_error("[MCTypesGenerator] Must be a MUL IOPERATOR");
    }

    if (stmt.get_type() == OPT_BYTE) {
//        MOperand op1 = parent->create_moperand(symbol(stmt.get_operand1()), stmt.get_type());
//        MOperand op2 = parent->create_moperand(symbol(stmt.get_operand2()), stmt.get_type());
//        MOperand rax = moperand(MOPF_REG, RN_RAX);
//        MOperand res = parent->create_moperand(symbol(stmt.get_result()), stmt.get_type());
//        info.mc.emplace_back(stmt.get_type(), stmt.get_is_signed(), MOP_MOVE, op1, rax);
//        info.mc.emplace_back(stmt.get_type(), stmt.get_is_signed(), MOP_MOVE, op2, res);
//        info.mc.emplace_back(stmt.get_type(), stmt.get_is_signed(), MOP_MUL, op2, res);
    } else {
        visit_binary(stmt.get_type(), stmt.get_is_signed(), MOP_MUL, stmt.get_operand1(),
                     stmt.get_operand2(), symbol(stmt.get_result()));
    }

    /**
     * Two cases:
     *  byte multiplication
     *      MOVE OP1 RAX
     *      MOVE OP2 RES
     *      IMUL RES
     *  normal multiplication
     *      MOVE OP1 RES
     *      IMUL OP2 RES
     */
}