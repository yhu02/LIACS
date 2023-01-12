/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_IOPERATOR_H
#define BUILD_IOPERATOR_H

#include <assignmenttype.h>
#include "types.h"
#include "to_string.h"
#include <iostream>

/**
 * File contains an enum to represent the IOperator-types used in IStatements
 * Also contains 'to_string' functionality
 */

typedef enum ioperator {
    /**
     * Assignment 1: Calculator
     */
    /* Meta */
    IOP_UNKNOWN = 0, // [---] (yet) Unknown operation

    /* Binary arithmetic operators
     *  operand1    the first source operand
     *  operand2    the second source operand
     *  result      the destination operand */
    IOP_ADD,    // [12r] Addition
    IOP_SUB,    // [12r] Subtraction
    IOP_MUL,    // [12r] Multiplication

    /* Unary arithmetic operators
     *  operand1    the source operand
     *  result      the destination operand */
    IOP_UNARY_MINUS,

    /* Assignment operator
     *  operand1    the source operand
     *  result      the destination operand */
    IOP_ASSIGN,

    /* Print operator
     * operand1      the operand to print
     */
    IOP_PRINT,

    /**
     * Assignment 2: Dangling Else
     */

    /* Label
     operand1  should refer to the corresponding Symbol object */
    IOP_LABEL, // [1--] Label

    /* Goto
         operand1  should refer to the corresponding Symbol object; note that
                             the Symbol pointed to should be of type ST_LABEL */
    IOP_GOTO, // [1--] Goto a label

    /* Branching
     operand1  the first source operand
     operand2  the second source operand, should be a Symbol
     result    should refer to the target Symbol object; note that
                         the Symbol pointed to must be of type ST_LABEL */
    IOP_JE,               // [12r] == jump
    IOP_JNE,              // [12r] != jump
    IOP_JL,               // [12r] < signed jump
    IOP_JGE,              // [12r] >= signed jump
    IOP_JLE,              // [12r] <= signed jump
    IOP_JG,               // [12r] > signed jump

    /**
     * Assignment 4: Basic Compiler
     */
    /* Binary arithmetic operators
     *  operand1    the first source operand
     *  operand2    the second source operand
     *  result      the destination operand */
    IOP_DIV,              // [12r] Divide
    IOP_MOD               // [12r] Modulo
} IOperator;

typedef enum __operatortype {
    OPT_VOID = 0,
    OPT_BYTE,
    OPT_WORD,
    OPT_DOUBLE,
    OPT_QUAD
} OperatorType;

inline std::ostream& operator<<(std::ostream& stream, const __operatortype& type) {
    switch (type) {
        case OPT_VOID: break;
        case OPT_BYTE: stream << "b"; break;
        case OPT_WORD: stream << "w"; break;
        case OPT_DOUBLE: stream << "l"; break;
        case OPT_QUAD: stream << "q"; break;
    }
    return stream;
}

namespace util {
    template<>
    std::string to_string<IOperator>(IOperator);
    template<>
    std::string to_string<OperatorType>(OperatorType);

    OperatorType from_return_type(ReturnType);
    size_t get_width(OperatorType);
    bool is_signed(ReturnType);
    AssignmentType get_assignment_type(IOperator);
}

#endif //BUILD_IOPERATOR_H
