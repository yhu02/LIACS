#ifndef BUILD_IFELSEGENERATOR_H
#define BUILD_IFELSEGENERATOR_H

#include <src/main/cpp/machinecode/mcgenerators/mcgenerator.h>

class MCConditionalGenerator : public virtual MachineCodeGenerator {
public:
    explicit MCConditionalGenerator(IntermediateCodeVisitor* parent, IntermediateCodeVisitorInfo& info) :
            MachineCodeGenerator(parent, info) {}

    void accept(const IStatement& stmt) override;
};

#endif //BUILD_IFELSEGENERATOR_H
