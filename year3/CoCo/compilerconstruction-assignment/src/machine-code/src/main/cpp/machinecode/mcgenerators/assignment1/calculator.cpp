#include "calculator.h"
#include "../../intermediatecodevisitor.h"

void MCCalculatorGenerator::accept(const IStatement &stmt) {
    switch (stmt.get_ioperator()) {
        case IOP_UNKNOWN:
            info.logger.warn(-1) << "Encountered Unknown Operator";
            break;
        case IOP_ADD:
            visit_binary(stmt.get_type(), stmt.get_is_signed(), MOP_ADD, stmt.get_operand1(), stmt.get_operand2(),
                         symbol(stmt.get_result()));
            break;
        case IOP_SUB:
            visit_binary(stmt.get_type(), stmt.get_is_signed(), MOP_SUB, stmt.get_operand1(), stmt.get_operand2(),
                         symbol(stmt.get_result()));
            break;
        case IOP_MUL:
            visit_binary(stmt.get_type(), stmt.get_is_signed(), MOP_MUL, stmt.get_operand1(), stmt.get_operand2(),
                         symbol(stmt.get_result()));
            break;
        case IOP_UNARY_MINUS:
            visit_unary_minus(stmt.get_type(), stmt.get_is_signed(), stmt.get_operand1(), symbol(stmt.get_result()));
            break;
        case IOP_ASSIGN:
            visit_move(stmt.get_type(), stmt.get_is_signed(), stmt.get_operand1(), symbol(stmt.get_result()));
            break;
        case IOP_PRINT:
            visit_print(stmt.get_type(), stmt.get_is_signed(), stmt.get_operand1());
            break;
        default:
            throw std::runtime_error("[MCCalculatorGenerator] Cannot process statement");
    }
    ++info.lineno;
}

void MCCalculatorGenerator::visit_unary_minus(OperatorType type, bool is_signed, const IOperand* operand, const SymbolIOperand* result) {    
    /**
     * MOVE OP RES
     * MOP RES
     */
    visit_move(type, is_signed, operand, result);
    info.mc.emplace_back(type, is_signed, MOP_NEG, empty_moperand(), parent->create_moperand(result, type));
}