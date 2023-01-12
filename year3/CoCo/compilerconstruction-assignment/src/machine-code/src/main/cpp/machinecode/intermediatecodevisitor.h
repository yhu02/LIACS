#ifndef BUILD_MCVISITOR_H
#define BUILD_MCVISITOR_H

#include "src/main/cpp/machinecode/mcgenerators/mcgenerator.h"
#include "./mcgenerators/assignment1/calculator.h"
#include "./mcgenerators/assignment2/conditionals.h"
#include "./mcgenerators/assignment3/types.h"
#include "./mcgenerators/assignment4/divmod.h"
#include "../registerallocators/betterregisterallocator.h"
#include "../stackallocators/betterstackallocator.h"

/**
 * File containing entry-point to transform intermediate-code to machine-code
 */


/**
 * Info required by all intermediate-code visitors
 */
struct IntermediateCodeVisitorInfo {
    IntermediateCodeVisitorInfo(Logger& logger, RegisterAllocator* allocator) : logger(logger), registerAllocator(allocator) {}

    Logger& logger;
    int lineno = 1; // The instruction we are handling
    std::vector<Instruction> mc = {}; // generated MStatements
    RegisterAllocator* registerAllocator;
};


/**
 * Entry-point for transforming intermediate-code into machine-code
 * Class is responsible for propagating each IStatement to the right assignment-specific visitor
 */
class IntermediateCodeVisitor {
public:
    std::vector<Instruction> retrieve_mc() { return std::move(info.mc); }

    size_t retrieve_stack_size() {
        // from: https://stackoverflow.com/a/3407254
        size_t num_to_round = stackAllocator->get_allocated();
        size_t remainder = num_to_round % STACK_MULTIPLE;
        if (remainder == 0)
            return num_to_round;
        return num_to_round + STACK_MULTIPLE - remainder;
    }

    // TODO: assign StackAllocator and RegisterAllocator on usage
    explicit IntermediateCodeVisitor(Logger& logger) :
    registerAllocator(new BetterRegisterAllocator(logger)),
    stackAllocator(new BetterStackAllocator(logger)),
    info(logger, registerAllocator) {
        this->ass1 = std::make_unique<MCCalculatorGenerator>(this, info);
        this->ass2 = std::make_unique<MCConditionalGenerator>(this, info);
        this->ass3 = std::make_unique<MCTypesGenerator>(this, info);
        this->ass4 = std::make_unique<MCDivModGenerator>(this, info);
    }

    ~IntermediateCodeVisitor() { delete registerAllocator; }

    void accept(const IStatement& stmt) {
        switch (stmt.get_assignment_type()) {
            case ASS_FIVE: break;
            case ASS_FOUR: ass4->accept(stmt); break;
            case ASS_THREE: ass3->accept(stmt); break;
            case ASS_TWO: ass2->accept(stmt); break;
            case ASS_ONE: ass1->accept(stmt); break;
        }
        ++info.lineno;
    }

    MOperand create_moperand(const SymbolIOperand* sym, OperatorType type) {
        if (CURRENT_ASSIGNMENT & ASS_FIVE) {
            return stackAllocator->to_memory(sym, util::get_width(type), info.lineno);
        } else {
            (void)type; // make sure we do not get compile-warnings :)
            return registerAllocator->to_register(sym, info.lineno);
        }
    }
protected:
    RegisterAllocator* registerAllocator;
    StackAllocator* stackAllocator;
    IntermediateCodeVisitorInfo info;

    std::unique_ptr<MachineCodeGenerator> ass1 = nullptr;
    std::unique_ptr<MachineCodeGenerator> ass2 = nullptr;
    std::unique_ptr<MachineCodeGenerator> ass3 = nullptr;
    std::unique_ptr<MachineCodeGenerator> ass4 = nullptr;
};

#endif //BUILD_MCVISITOR_H
