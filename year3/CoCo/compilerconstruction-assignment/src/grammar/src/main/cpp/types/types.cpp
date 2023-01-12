/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "types.h"

template<>
std::string util::to_string<NodeType>(NodeType nodetype) {
    switch (nodetype) {
        case NODE_UNKNOWN:      return "Unknown";
        case NODE_ERROR:        return "Error";
        case NODE_ADD:          return "Addition";
        case NODE_SUB:          return "Subtraction";
        case NODE_MUL:          return "Multiplication";
        case NODE_SIGNPLUS:     return "Unary plus";
        case NODE_SIGNMINUS:    return "Unary minus";
        case NODE_NUM:          return "Constant number";
        case NODE_EQ:           return "Equality";
        case NODE_NEQ:          return "Non-Equality";
        case NODE_GT:           return "Greater-Than";
        case NODE_GEQ:          return "Greater-Equal";
        case NODE_LT:           return "Less-Than";
        case NODE_LEQ:          return "Less-Equal";
        case NODE_PRINT:        return "Print";
        case NODE_IF:           return "If-then";
        case NODE_IF_ELSE:      return "If-else";
        case NODE_ASSIGNMENT:   return "Assignment";
        case NODE_LIST:         return "List";
        case NODE_ID:           return "Identifier";
        case NODE_EMPTY:        return "Empty";
        case NODE_WHILE:        return "While";
        case NODE_DIV:          return "Divide";
        case NODE_MOD:          return "Modulo";
    }
    return "??";
}

template<>
std::string util::to_string<ReturnType>(ReturnType object) {
    switch (object) {
        case RT_UNKNOWN:        return "Unknown";
        case RT_ERROR:          return "Error";
        case RT_BOOL:           return "Boolean";
        case RT_VOID:           return "Void";
        case RT_INT:            return "Int";
        case RT_INT8:           return "Int8";
        case RT_UINT:           return "Unsigned";
        case RT_UINT8:          return "UInt8";
    }
    return "??";
}

bool util::is_binary_operator(NodeType nt) {
    return (nt <= NODE_MUL && nt >= NODE_ADD) || nt == NODE_DIV || nt == NODE_MOD;
}

bool util::is_boolean_operator(NodeType nt) {
    return nt <= NODE_LEQ && nt >= NODE_EQ;
}

bool util::is_statement(NodeType nt) {
    return (nt >= NODE_IF && nt <= NODE_LIST) || nt == NODE_WHILE;
}

AssignmentType util::get_assignment_type(NodeType nodetype) {
    if (nodetype <= NODE_NUM)
        return ASS_ONE;
    else if (nodetype <= NODE_IF_ELSE)
        return ASS_TWO;
    else if (nodetype <= NODE_EMPTY)
        return ASS_THREE;
    return ASS_FOUR;
}