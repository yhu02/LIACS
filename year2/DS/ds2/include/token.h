#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include "./constants.h"

class Token{
    public:
    Token(TYPE type = TYPE::INVALID, std::string value = "", std::string error = "");

    TYPE getType() const;
    std::string getValue() const;
    std::string getError() const;
    void setType(TYPE type);
    void setValue(std::string value);
    void setError(std::string error);
    
    private:
    TYPE type;
    std::string value;
    std::string error;
};

//Token operator overloading
bool operator==(Token const token, std::string const str);
bool operator==(Token const token, TYPE const type);
bool operator==(Token const token, Token const token2);
bool operator!=(Token const token, std::string const str);
bool operator!=(Token const token, TYPE const type);
bool operator!=(Token const token, Token const token2);

#endif