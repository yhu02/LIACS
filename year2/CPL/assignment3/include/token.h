/**
 * Description: delcaration of the token class
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file hulpfuncties.h
 **/

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

enum class TYPE{
    ARROW,
    LPAR,
    RPAR,
    LAMBDA,
    DOT,
    LVAR,
    UVAR,
    CARET,
    COLON,
    APPLY,
    eof
};

class Token{
    public:
    Token(TYPE type, std::string value);

    TYPE getType() const;
    std::string getValue() const;
    void setType(TYPE type);
    void setValue(std::string value);
    
    private:
    TYPE type;
    std::string value;
};

//Token operator overloading
bool operator==(Token const token, std::string const str);
bool operator==(Token const token, TYPE const type);
bool operator==(Token const token, Token const token2);
bool operator!=(Token const token, std::string const str);
bool operator!=(Token const token, TYPE const type);
bool operator!=(Token const token, Token const token2);

#endif