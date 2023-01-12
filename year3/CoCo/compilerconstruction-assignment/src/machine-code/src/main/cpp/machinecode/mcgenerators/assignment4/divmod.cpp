#include "../../intermediatecodevisitor.h"
#include "divmod.h"

void MCDivModGenerator::accept(const IStatement &stmt) {
    // TODO: Implement in Assignment 4
    // Hint: Use the visit_div method

    if (stmt.get_ioperator() == IOP_DIV) {
        visit_div(stmt.get_type(), stmt.get_is_signed(), stmt.get_operand1(), symbol(stmt.get_operand2()), symbol(stmt.get_result()),
                  nullptr);
    } else if (stmt.get_ioperator() == IOP_MOD) {
        visit_div(stmt.get_type(), stmt.get_is_signed(), stmt.get_operand1(), symbol(stmt.get_operand2()), nullptr,
                  symbol(stmt.get_result()));
    }

    (void) stmt;
}

// https://stackoverflow.com/questions/8021772/assembly-language-how-to-do-modulo
void MCDivModGenerator::visit_div(OperatorType type, bool is_signed,
                                  const IOperand *operand1,
                                  const SymbolIOperand *operand2,
                                  const SymbolIOperand *div_result,
                                  const SymbolIOperand *mod_result) {
    // TODO: Implement in Assignment 4
    MOperand op1 = (operand1->getOperandType() == OT_SYMBOL)
                   ? parent->create_moperand(symbol(operand1), type)
                   : to_immediate(immediate(operand1));

    MOperand op2 = (operand2->getOperandType() == OT_SYMBOL)
                   ? parent->create_moperand(symbol(operand2), type)
                   : to_immediate(immediate(operand2));

    moperand rax = moperand(MOPF_REG, RN_RAX);
    moperand rdx = moperand(MOPF_REG, RN_RDX);

    info.mc.emplace_back(type, is_signed, MOP_MOVE, op1, rax);

    if (is_signed) {
        info.mc.emplace_back(type, is_signed, MOP_CONV, empty_moperand(), empty_moperand());
    } else {
        info.mc.emplace_back(type, is_signed, MOP_XOR, rdx, rdx);
    }

    info.mc.emplace_back(type, is_signed, MOP_DIV, op2, rax);
    if (mod_result != nullptr) {
        MOperand mod_res = parent->create_moperand(mod_result, type);
        info.mc.emplace_back(type, is_signed, MOP_MOVE, rdx, mod_res);
    }
    if (div_result != nullptr) {
        MOperand div_res = parent->create_moperand(div_result, type);
        info.mc.emplace_back(type, is_signed, MOP_MOVE, rax, div_res);
    }

    /**
     * Note: we require RAX and RDX
     *
     * 1. Move operand1 to RAX
     * 2. zero-extend or sign-extend EAX to EDX:EAX, through
     *  - Zero-extend: xor %edx, %edx
     *  - Sign-extend: cdq
     * 3. Perform 'div op2', or 'idiv op2'
     * 4. Move EDX to mod_result
     * 5. Move EAX to div_result
     *
     * from stackoverflow:
     * "Note that 8-bit operand-size is special: the implicit inputs/outputs are
     * in AH:AL (aka AX), not DL:AL"
     */
}