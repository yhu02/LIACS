#include "../include/token.h"

Token::Token(TYPE type, std::string value, std::string error){
    this->type = type;
    this->value = value;
    this->error = error;
}

TYPE Token::getType() const {
    return this->type;
}
std::string Token::getValue() const {
    return this->value;
}
std::string Token::getError() const {
    return this->error;
}
void Token::setType(TYPE type){
    this->type = type;
}
void Token::setValue(std::string value){
    this->value = value;
}
void Token::setError(std::string error){
    this->error = error;
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
