/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "ioperator.h"

template<>
std::string util::to_string<IOperator>(IOperator op) {
    switch (op) {
        case IOP_UNKNOWN: return "Unknown";
        case IOP_ADD: return "ADD";
        case IOP_SUB: return "SUB";
        case IOP_MUL: return "MUL";
        case IOP_UNARY_MINUS: return "UMINUS";
        case IOP_ASSIGN: return "ASSIGN";
        case IOP_LABEL: return "LABEL";
        case IOP_GOTO: return "GOTO";
        case IOP_JE: return "JUMP EQUAL";
        case IOP_JNE: return "JUMP NOT EQUAL";
        case IOP_JL: return "JUMP LESS EQUAL";
        case IOP_JGE: return "JUMP GREATER EQUAL";
        case IOP_JLE: return "JUMP LESS EQUAL";
        case IOP_JG: return "JUMP GREATER";
        case IOP_PRINT: return "PRINT";
        case IOP_DIV: return "DIV";
        case IOP_MOD: return "MOD";
    }
    return "??";
}

template<>
std::string util::to_string<OperatorType>(OperatorType type) {
    switch (type) {
        case OPT_VOID: return "VOID";
        case OPT_BYTE: return "BYTE";
        case OPT_WORD: return "WORD";
        case OPT_DOUBLE: return "DOUBLE";
        case OPT_QUAD: return "QUAD";
    }
    return "??";
}

OperatorType util::from_return_type(ReturnType type) {
    switch (type) {
        case RT_UNKNOWN:
            throw std::runtime_error("Cannot convert unknown return type (RT_UNKNOWN) to operator type");
        case RT_ERROR:
            throw std::runtime_error("Cannot convert error return type (RT_ERROR) to operator type");
        case RT_VOID:
            return OPT_VOID;
        case RT_BOOL:
        case RT_INT8:
        case RT_UINT8:
            return OPT_BYTE;
        case RT_INT:
        case RT_UINT:
            return OPT_DOUBLE;
    }
    return OPT_VOID;
}

size_t util::get_width(OperatorType type) {
    switch (type) {
        case OPT_VOID: return 0;
        case OPT_BYTE: return 1 << 0;
        case OPT_WORD: return 1 << 1;
        case OPT_DOUBLE: return 1 << 2;
        case OPT_QUAD: return 1 << 3;
    }
    return 0;
}

bool util::is_signed(ReturnType type) {
    return type != RT_UINT8 && type != RT_UINT;
}

AssignmentType util::get_assignment_type(IOperator op) {
    if (op == IOP_MUL && (CURRENT_ASSIGNMENT & ASS_THREE))
        return ASS_THREE;

    if (op <= IOP_PRINT)
        return ASS_ONE;
    if (op <= IOP_JG)
        return ASS_TWO;
    return ASS_FOUR;
}