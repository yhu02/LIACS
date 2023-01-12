#ifndef BUILD_MCGENERATOR_H
#define BUILD_MCGENERATOR_H

#include <algorithm>
#include <vector>
#include <istatement.h>
#include <instruction.h>

#include "logger.h"
#include "src/main/cpp/registerallocators/registerallocator.h"

class IntermediateCodeVisitor;
struct IntermediateCodeVisitorInfo;

/**
 * Generic MachineCodeGenerator to transform intermediate-code to machine-code
 */

class MachineCodeGenerator {
public:
    explicit MachineCodeGenerator(IntermediateCodeVisitor* parent, IntermediateCodeVisitorInfo& info) : parent(parent), info(info){}
    virtual ~MachineCodeGenerator() = default;

    /**
     * 'Visits' the IStatement to generate an MStatement
     * uses https://www.felixcloutier.com/x86/ for documentation
     * NOTE: all instructions can have at most one memory location
     * @param stmt
     */
     virtual void accept(const IStatement& stmt) = 0;

protected:
    IntermediateCodeVisitor* parent;
    IntermediateCodeVisitorInfo& info;

    static auto symbol(const IOperand *op) {
        return dynamic_cast<const SymbolIOperand *>(op);
    };
    static auto immediate(const IOperand *op) {
        return dynamic_cast<const ImmediateIOperand *>(op);
    };

    /**
     * Converts an ImmediateIOperand to a MOperand
     * Fails if the ImmediateIOperand is a nullptr
     * @param imm the ImmediateIOperand to convert
     * @return the converted MOperand, or an error_moperand() on error
     */
    MOperand to_immediate(const ImmediateIOperand *imm) const;

    /**
     * Visits a Binary Instruction and generates its corresponding
     * MachineCode-Instruction
     * @param mOperator MachineCode-Operator to generate
     * @param operand1 First operand of the IntermediateCode-Instruction
     * @param operand2 Second operand of the IntermediateCode-Instruction
     * @param result Result operand of the IntermediateCode-Instruction. Since
     * this represents a register, this should be a SymbolIOperand
     */
    void visit_binary(OperatorType type, bool is_signed, MOperator mOperator,
                        const IOperand *operand1, const IOperand *operand2,
                        const SymbolIOperand *result);

    /**
     * Generate a MOV operation from the operand to the result
     * @param source Source operand of the IntermediateCode-Instruction
     * @param destination Destination operand of the IntermediateCode-Instruction.
     * Since this represents a register, this should be a SymbolIOperand
     */
    void visit_move(OperatorType type, bool is_signed, const IOperand *source,
                    const SymbolIOperand *destination);

    /**
     * Generate a Label Instruction
     * @param label the Operand of the label
     */
    void generate_label(OperatorType type, bool is_signed, MOperator mOperator, const SymbolIOperand* label);

    /**
     * Generate Compare-and-Jump Instructions
     * @param moperator compare to make
     * @param operand1 first operand to compare against second operand, should be a symbol
     * @param operand2 second operand to compare against first operand
     * @param label label to jump to
     */
    void visit_jump(OperatorType type, bool is_signed, MOperator moperator, const IOperand* operand1, const SymbolIOperand* operand2, const SymbolIOperand* label);

    /**
     * Prepares a print instruction
     * @param operand the operand to print
     */
    void visit_print(OperatorType type, bool is_signed, const IOperand *operand);
};

#endif //BUILD_MCGENERATOR_H
