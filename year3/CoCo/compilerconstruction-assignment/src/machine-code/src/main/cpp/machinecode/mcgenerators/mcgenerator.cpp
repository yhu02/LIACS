#include "mcgenerator.h"
#include "../intermediatecodevisitor.h"

// TODO: implement functions here

MOperand
MachineCodeGenerator::to_immediate(const ImmediateIOperand *imm) const {
    if (imm == nullptr) {
        info.logger.error(info.lineno) << "operand is not an ImmediateIOperand\n";
        return error_moperand();
    }
    return {MOPF_VAL, imm->get_value()};
}


void MachineCodeGenerator::visit_move(OperatorType type, bool is_signed,
                                      const IOperand *source,
                                      const SymbolIOperand *destination) {
    /**
     * MOVE SRC DST
     */
    if (source == nullptr || source->getOperandType() == OT_UNKNOWN) {
        info.logger.error(info.lineno) << " invalid source for move instruction\n";
        return;
    }
    MOperand src = (source->getOperandType() == OT_SYMBOL) ? parent->create_moperand(symbol(source), type) : to_immediate(immediate(source));
    MOperand dst = parent->create_moperand(destination, type);
    if (src.flag == MOPF_MEM && dst.flag == MOPF_MEM) {
        MOperand dst_reg = moperand(MOPF_REG, RN_RAX);
        info.mc.emplace_back(type, is_signed, MOP_MOVE, src, dst_reg);
        info.mc.emplace_back(type, is_signed, MOP_MOVE, dst_reg, dst);
    } else {
        info.mc.emplace_back(type, is_signed, MOP_MOVE, src, dst);
    }
}


void MachineCodeGenerator::visit_binary(OperatorType type, bool is_signed,
                                        MOperator mOperator,
                                        const IOperand *operand1,
                                        const IOperand *operand2,
                                        const SymbolIOperand *result) {
    /**
     * MOVE OP1 RES
     * MOP OP2 RES
     */
    visit_move(type, is_signed, operand1, result);
    if (operand2 == nullptr || operand2->getOperandType() == OT_UNKNOWN) {
        info.logger.error(info.lineno) << "invalid operand2 for " << util::to_string(mOperator) << "\n";
        return;
    }
    MOperand op2 = (operand2->getOperandType() == OT_SYMBOL) ? parent->create_moperand(symbol(operand2), type) : to_immediate(immediate(operand2));
    MOperand res = parent->create_moperand(result, type);
    if (mOperator != MOP_MOVE && res.flag == MOPF_MEM) {
        MOperand bridge_reg = moperand(MOPF_REG, RN_RAX);
        info.mc.emplace_back(type, is_signed, MOP_MOVE, res, bridge_reg);
        info.mc.emplace_back(type, is_signed, mOperator, op2, bridge_reg);
        info.mc.emplace_back(type, is_signed, MOP_MOVE, bridge_reg, res);
    } else {
        info.mc.emplace_back(type, is_signed, mOperator, op2, res);
    }
}

void MachineCodeGenerator::generate_label(OperatorType type, bool is_signed, MOperator mOperator,
                                          const SymbolIOperand *label) {
    // TODO: implement in Assignment 2

    MOperand res = parent->create_moperand(label, type);
    info.mc.emplace_back(type, is_signed, mOperator, res, empty_moperand());
    (void) type;
    (void) is_signed;
    (void) mOperator;
    (void) label;
    
    /**
     * L{id}:
     */
}


void MachineCodeGenerator::visit_jump(OperatorType type, bool is_signed, MOperator moperator,
                                      const IOperand *operand1,
                                      const SymbolIOperand *operand2, const SymbolIOperand *label) {


    // TODO: implement in Assignment 2
    MOperand op1 = (operand1->getOperandType() == OT_SYMBOL)
                   ? parent->create_moperand(symbol(operand1), type)
                   : to_immediate(immediate(operand1));

    MOperand op2 = (operand2->getOperandType() == OT_SYMBOL)
                   ? parent->create_moperand(symbol(operand2), type)
                   : to_immediate(immediate(operand2));

    
    
    MOperand res = parent->create_moperand(label, type);

    if(moperator == MOP_JMP)
    {
        info.mc.emplace_back(type, is_signed, moperator, res, empty_moperand());

    }else{
        info.mc.emplace_back(type, is_signed, MOP_COMP, op1, op2);
        info.mc.emplace_back(type, is_signed, moperator, res, empty_moperand());
    }
    return;
    
     
    /**
     * NOTE: if OP1 is an ImmediateIOperand --> turn it into register
     * 
     * CMP OP1 OP2
     * MOP LABEL
     */
}

void MachineCodeGenerator::visit_print(OperatorType type, bool is_signed,
                                       const IOperand *operand) {
    /**
     * MOV OP RDI
     * PRINT
     */
    MOperand op = (operand->getOperandType() == OT_SYMBOL)
                  ? parent->create_moperand(symbol(operand), type)
                  : to_immediate(immediate(operand));
    info.mc.emplace_back(type, is_signed, MOP_MOVE, op,
                         moperand(MOPF_REG, RN_RDI));
    info.mc.emplace_back(type, is_signed, MOP_PRINT, empty_moperand(),
                         empty_moperand());
}