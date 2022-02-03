/**
 * Description: Implementation of the token class
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file token.cc
 **/

#include "token.h"

Token::Token(TYPE type, std::string value):
    type(type), value(value)
{

}

TYPE Token::getType() const {
    return this->type;
}
std::string Token::getValue() const {
    return this->value;
}
void Token::setType(TYPE type){
    this->type = type;
}
void Token::setValue(std::string value){
    this->value = value;
}

//Token operator overloading
bool operator==(Token const token, std::string const str){
    return token.getValue() == str;
}
bool operator==(Token const token, TYPE const type){
    return token.getType() == type;
}
bool operator==(Token const token, Token const token2){
    return token.getValue() == token2.getValue();
}
bool operator!=(Token const token, std::string const str){
    return token.getValue() != str;
}
bool operator!=(Token const token, TYPE const type){
    return token.getType() != type;
}
bool operator!=(Token const token, Token const token2){
    return token.getValue() != token2.getValue();
}
