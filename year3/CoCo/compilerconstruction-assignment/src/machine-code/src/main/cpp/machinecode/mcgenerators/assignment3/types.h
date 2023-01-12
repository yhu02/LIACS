#ifndef BUILD_TYPESGENERATOR_H
#define BUILD_TYPESGENERATOR_H

#include <src/main/cpp/machinecode/mcgenerators/mcgenerator.h>

class MCTypesGenerator : public virtual MachineCodeGenerator {
public:
    explicit MCTypesGenerator(IntermediateCodeVisitor* parent, IntermediateCodeVisitorInfo& info) :
            MachineCodeGenerator(parent, info) {}

    void accept(const IStatement& stmt) override;
};

#endif //BUILD_TYPESGENERATOR_H
