#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "./token.h"

class Expression{
    public:
    Token token;
    std::vector<Expression> expressions;
};

#endif