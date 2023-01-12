/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_MACHINECODE_H
#define BUILD_MACHINECODE_H

#include <logger.h>
#include <intermediatecode.h>
#include "instruction.h"

/**
 * This file contains a class to represent MachineCode
 * This class generates an iterable MachineCode object from IntermediateCode
 * For this, the class has its own Iterator
 */

class MachineCode {
public:
    /**
     * Constructs the MachineCode object from the provided IntermediateCode
     * @param logger Logger to log messages to
     * @param ic IntermediateCode object to generate MachineCode from
     */
    MachineCode(Logger& logger, const IntermediateCode& ic);

    /**
     * Iterator to iterate over all instructions
     * Note: This is a const forward-Iterator; We do not allow any modifications and can only iterate forward
     */
    struct Iterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const std::string;
        using pointer = const std::string*;
        using reference = const std::string&;

        explicit Iterator();
        explicit Iterator(std::vector<Instruction>::iterator begin, std::vector<Instruction>::iterator end);
        ~Iterator() { delete m_ptr; }
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }
        Iterator& operator++();
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }
    private:
        pointer m_ptr;
        std::vector<Instruction>::iterator instruction;
        std::vector<Instruction>::iterator end;
    };

    Iterator begin();
    static Iterator end();

    /**
     * @return the number of bytes required for the stack
     */
    [[nodiscard]] size_t get_allocated() const { return allocated; }
private:
    std::vector<Instruction> instructions;
    size_t allocated = 0;
};


#endif //BUILD_MACHINECODE_H
