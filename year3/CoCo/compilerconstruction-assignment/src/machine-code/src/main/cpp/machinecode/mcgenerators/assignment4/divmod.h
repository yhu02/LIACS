#ifndef BUILD_ASS4GENERATOR_H
#define BUILD_ASS4GENERATOR_H

#include "src/main/cpp/machinecode/mcgenerators/mcgenerator.h"

class MCDivModGenerator : public virtual MachineCodeGenerator {
public:
    explicit MCDivModGenerator(IntermediateCodeVisitor* parent, IntermediateCodeVisitorInfo& info) :
            MachineCodeGenerator(parent, info) {}

    void accept(const IStatement& stmt) override;

protected:
    void visit_div(OperatorType type, bool is_signed, const IOperand* operand1, const SymbolIOperand* operand2,
                   const SymbolIOperand* div_result, const SymbolIOperand* mod_result);
};

#endif //BUILD_ASS4GENERATOR_H
