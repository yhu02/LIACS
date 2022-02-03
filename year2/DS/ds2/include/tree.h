#ifndef TREE_H
#define TREE_H
#include <vector>
#include <iostream>
#include <fstream>
#include "./node.h"
#include "./functions.h"
#include "./constants.h"

class Tree{
    public:
    Tree(Node* top = nullptr);
    ~Tree();

    void print(Node* node) const;
    void dot(std::string fileName);
    void simp(Node* node);
    void eval(std::vector<Token> tokens);
    void diff(Node* node, std::string variable, Node* parent = nullptr);
    bool setTop(Node* top);
    Node* getTop() const;

    private:
    bool divisionByZero;            //Check if a division by zero has been detected by simp
    int nodeI;                      //Keep track of nodes by dot
    Node* top;                      //Top of current tree
    std::string input;
    std::vector<Token> tokens;

    //Functions related to simp
    void treeCalculate(Node* node);
    void treeOperator(Node* node);
    void treeExponent(Node* node);
    void treeMultiplication(Node* node);
    void treeMultiplicationRec(Node* node, Node* nodeParent, Node* match, Node* matchParent, std::string operator_);
    void treeMultiplicationCalculate(Node* node, Node* nodeParent, Node* match, Node* matchParent);
    void treeDivision(Node* node);
    void treeAddition(Node* node);
    void treeTrig(Node* node);
    
    //Functions related to diff
    void treeDiffOperator(Node* node, std::string variable);
    void treeDiffFunction(Node* node, std::string variable, Node* parent);
    void treeDiffMultiplication(Node* node, std::string variable);
    void treeDiffDivision(Node* node, std::string variable);
    void treeDiffTrig(Node* node, Node* parent);
    void treeDiffExponent(Node* node, std::string variable);

    //Functions related to dot
    std::string dotRecursive(Node* node, int parent);

};

#endif