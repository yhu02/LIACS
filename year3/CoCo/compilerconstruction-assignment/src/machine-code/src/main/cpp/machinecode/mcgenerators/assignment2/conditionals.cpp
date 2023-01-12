#include "conditionals.h"
#include "../../intermediatecodevisitor.h"

void MCConditionalGenerator::accept(const IStatement &stmt) {
    // TODO: Implement in Assignment 2
    switch (stmt.get_ioperator()) {
        case IOP_UNKNOWN:
            info.logger.warn(-1) << "Encountered Unknown Operator";
            break;
        case IOP_GOTO:
            visit_jump(stmt.get_type(), stmt.get_is_signed(), MOP_JMP, stmt.get_operand1(),
                        symbol(stmt.get_operand2()), symbol(stmt.get_result()));
            break;
        case IOP_JE:
            visit_jump(stmt.get_type(), stmt.get_is_signed(), MOP_JE, stmt.get_operand1(),
                        symbol(stmt.get_operand2()), symbol(stmt.get_result()));
            break;
        case IOP_JNE:
            visit_jump(stmt.get_type(), stmt.get_is_signed(), MOP_JNE, stmt.get_operand1(),
                        symbol(stmt.get_operand2()), symbol(stmt.get_result()));
            break;
        case IOP_JG:
            visit_jump(stmt.get_type(), stmt.get_is_signed(), MOP_JG, stmt.get_operand1(),
                        symbol(stmt.get_operand2()), symbol(stmt.get_result()));
            break;
        case IOP_JGE:
            visit_jump(stmt.get_type(), stmt.get_is_signed(), MOP_JGE, stmt.get_operand1(),
                        symbol(stmt.get_operand2()), symbol(stmt.get_result()));
            break;
        case IOP_JL:
            visit_jump(stmt.get_type(), stmt.get_is_signed(), MOP_JL, stmt.get_operand1(),
                        symbol(stmt.get_operand2()), symbol(stmt.get_result()));
            break;
        case IOP_JLE:
            visit_jump(stmt.get_type(), stmt.get_is_signed(), MOP_JLE, stmt.get_operand1(),
                        symbol(stmt.get_operand2()), symbol(stmt.get_result()));
            break;
        case IOP_LABEL:
            generate_label(stmt.get_type(), stmt.get_is_signed(), MOP_LABEL, symbol(stmt.get_operand1()));
            break;
        default:
            throw std::runtime_error("[MCCalculatorGenerator] Cannot process statement");
    }
    ++info.lineno;
}


