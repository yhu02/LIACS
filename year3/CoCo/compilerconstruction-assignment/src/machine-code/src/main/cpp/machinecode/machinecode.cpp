/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "machinecode.h"
#include "intermediatecodevisitor.h"

#include <functional>
#include <sstream>


MachineCode::MachineCode(Logger& logger, const IntermediateCode& ic)  {
    IntermediateCodeVisitor vis(logger);
    for (const IStatement& stmt : ic)
        vis.accept(stmt);
    instructions = vis.retrieve_mc();
    allocated = vis.retrieve_stack_size();
}

MachineCode::Iterator MachineCode::begin() {
    return MachineCode::Iterator(instructions.begin(), instructions.end());
}

MachineCode::Iterator MachineCode::end() {
    return MachineCode::Iterator();
}

MachineCode::Iterator::Iterator(std::vector<Instruction>::iterator begin, std::vector<Instruction>::iterator end) : instruction(begin), end(end) {
    if (begin == end) {
        m_ptr = nullptr;
        return;
    }
    std::stringstream instr_ss;
    instr_ss << *instruction;
    m_ptr = new std::string(instr_ss.str());
}

MachineCode::Iterator::Iterator() {
    m_ptr = nullptr;
}

MachineCode::Iterator& MachineCode::Iterator::operator++() {
   delete m_ptr;
   if (++instruction == end) {
       m_ptr = nullptr;
       return *this;
   }
   std::stringstream  instr_ss;
   instr_ss << *instruction;
   m_ptr = new std::string(instr_ss.str());
   return *this;
}
