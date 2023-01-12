/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_TYPES_H
#define BUILD_TYPES_H

#include <string>
#include <to_string.h>
#include <assignmenttype.h>

/**
 * This file contains an Enum to specify several Node-types
 * Additionally, we implemented a 'to_string' functionality
 */

typedef enum nodetype {
    /**
     * Assignment 1: Calculator
     */
    /* Meta */
    NODE_UNKNOWN = 0,   // Unknown (yet)
    NODE_ERROR,         // Error

    /* BinaryNode operators
     * left child:  left-hand side of the operation
     * right child: right-hand side of the operation */
    NODE_ADD,           // + Add
    NODE_SUB,           // - Subtract
    NODE_MUL,           // * Multiply

    /* UnaryNode
     * child: the subtree to which the operation has to be applied */
    NODE_SIGNPLUS,      // Unary Plus
    NODE_SIGNMINUS,     // Unary Minus
    NODE_PRINT,         // Print statement

    /* Leafs */
    NODE_NUM,            // Constant number

    /**
     * Assignment 2: Dangling Else
     */
    /* BinaryNode operators
     * left child:  left-hand side of the operation
     * right child: right-hand side of the operation */
    NODE_EQ,            // == Equality
    NODE_NEQ,           // != Non-Equality
    NODE_GT,            // > Greater-than
    NODE_GEQ,           // >= Greater-equal
    NODE_LT,            // < Less-than
    NODE_LEQ,           // <= Less-equal

    /* If-then-statement (BinaryNode)
     * left child: condition
     * right child: statement */
    NODE_IF,            // if-then

    /* If-then-else-statement (TernaryNode)
     * left-child: condition
     * middle-child: then-statement
     * right-child: else-statement
     */
    NODE_IF_ELSE,       // if-then-else

    /**
     * Assignment 3: Variables and Types
     */
    /* Assignment (BinaryNode)
     * left child: identifier
     * right child: expression
     */
    NODE_ASSIGNMENT,    // assignment

    /*
     * List (BinaryNode)
     * left child: current item
     * right child: next List or nullptr
     */
    NODE_LIST,          // list

    /*
     * Leafs
     */
    NODE_ID,             // Identifier
    NODE_EMPTY,          // Empty node

    /**
     * Assignment 4: Basic Compiler
     */
    /*
     * While (BinaryNode)
     * left child: condition
     * right child: statement
     */
    NODE_WHILE,

    /* BinaryNode operators
     * left child:  left-hand side of the operation
     * right child: right-hand side of the operation */
    NODE_DIV,           // / Divide
    NODE_MOD            // % Modulo
} NodeType;

/**
 * Assignment 3: Variables and Types
 */
typedef enum returntype {
    RT_UNKNOWN,         // Unknown (yet)
    RT_ERROR,           // Error
    RT_BOOL,            // Boolean
    RT_VOID,            // Void
    RT_INT,             // Integer
    RT_INT8,            // 8 width Integer
    RT_UINT,            // Unsigned
    RT_UINT8            // 8 width Unsigned Integer
} ReturnType;

namespace util {
    template<>
    std::string to_string<NodeType>(NodeType);
    template<>
    std::string to_string<ReturnType>(ReturnType object);

    bool is_binary_operator(NodeType nt);
    bool is_boolean_operator(NodeType nt);
    bool is_statement(NodeType nt);

    AssignmentType get_assignment_type(NodeType);
}

#endif //BUILD_TYPES_H
