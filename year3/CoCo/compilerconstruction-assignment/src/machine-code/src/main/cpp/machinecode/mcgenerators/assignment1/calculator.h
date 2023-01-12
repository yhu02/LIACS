#ifndef BUILD_MCCALCULATORGENERATOR_H
#define BUILD_MCCALCULATORGENERATOR_H

#include <src/main/cpp/machinecode/mcgenerators/mcgenerator.h>

class MCCalculatorGenerator : public virtual MachineCodeGenerator {
public:
    explicit MCCalculatorGenerator(IntermediateCodeVisitor* parent, IntermediateCodeVisitorInfo& info) :
        MachineCodeGenerator(parent, info) {}

    void accept(const IStatement& stmt) override;

protected:
    /**
     * Visit an Unary Instruction and generate its corresponding MachineCode-Instruction
     * @param mOperator MachineCode-Operator to generate
     * @param operand Source operand of the IntermediateCode-Instruction
     * @param result Result operand of the IntermediateCode-Instruction. Since this represents a register, this
     * should be a SymbolIOperand
     */
    void visit_unary_minus(OperatorType type, bool is_signed, const IOperand* operand, const SymbolIOperand* result);
};

#endif //BUILD_MCCALCULATORGENERATOR_H
