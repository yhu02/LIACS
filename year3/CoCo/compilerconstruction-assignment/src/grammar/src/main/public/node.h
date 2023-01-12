/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_NODE_H
#define BUILD_NODE_H

#include <memory>

#include "types.h"
#include "syntaxtreevisitor.h"

/**
 * This file contains several Node classes which represent nodes in a SyntaxTree
 * Nodes are immutable; once constructed they cannot change
 * Use a NodeBuilder to get a mutable Node
 *
 * AbstractSyntaxTreeVisitors can be used to visit the nodes in a tree, one can then visit a node by calling
 * 'node.accept(visitor)'
 */

class Node {
public:
    virtual ~Node() = default;
    Node(NodeType nodeType, ReturnType rt, int line) : nodeType(nodeType), returnType(rt), line(line) {}

    virtual void accept(AbstractSyntaxTreeVisitor* vis) const = 0;

    [[nodiscard]] NodeType getNodeType() const { return nodeType; }
    [[nodiscard]] ReturnType getReturnType() const { return returnType; }
    [[nodiscard]] int getLineno() const { return line; }
    [[nodiscard]] AssignmentType getAssignmentType() const { return util::get_assignment_type(nodeType); };
private:
    NodeType nodeType = NODE_UNKNOWN;
    ReturnType returnType = RT_UNKNOWN;
    int line = 1;
};

class UnaryNode : public Node {
public:
    UnaryNode(NodeType node_type, ReturnType  return_type, std::unique_ptr<Node> child, int line) :
        Node(node_type, return_type, line), child(std::move(child)) {};

    void accept(AbstractSyntaxTreeVisitor* vis) const override { vis->visit(*this); }

    [[nodiscard]] const Node& get_child() const { return *child; }
private:
    std::unique_ptr<Node> child;
};

class BinaryNode : public Node {
public:
    BinaryNode(NodeType node_type, ReturnType return_type, std::unique_ptr<Node> left_child, std::unique_ptr<Node> right_child, int line) :
            Node(node_type, return_type, line), left_child(std::move(left_child)), right_child(std::move(right_child)) {}

    void accept(AbstractSyntaxTreeVisitor* vis) const override { vis->visit(*this); }

    [[nodiscard]] const Node& get_left_child() const { return *left_child; }
    [[nodiscard]] const Node& get_right_child() const { return *right_child; }
private:
    std::unique_ptr<Node> left_child;
    std::unique_ptr<Node> right_child;
};

class TernaryNode : public Node {
public:
    TernaryNode(NodeType node_type, ReturnType return_type, std::unique_ptr<Node> left_child, std::unique_ptr<Node> middle_child, std::unique_ptr<Node> right_child, int line) :
            Node(node_type, return_type, line), left_child(std::move(left_child)), middle_child(std::move(middle_child)), right_child(std::move(right_child)) {}

    void accept(AbstractSyntaxTreeVisitor* vis) const override { vis->visit(*this); }

    [[nodiscard]] const Node& get_left_child() const { return *left_child; }
    [[nodiscard]] const Node& get_middle_child() const { return *middle_child; }
    [[nodiscard]] const Node& get_right_child() const { return *right_child; }
private:
    std::unique_ptr<Node> left_child;
    std::unique_ptr<Node> middle_child;
    std::unique_ptr<Node> right_child;
};

class ConstantNode : public Node {
public:
    ConstantNode(ReturnType return_type, uint64_t value, int line): Node(NODE_NUM, return_type, line), value(value) {}

    void accept(AbstractSyntaxTreeVisitor* vis) const override { vis->visit(*this); }

    [[nodiscard]] uint64_t get_value() const { return value; };
private:
    uint64_t value;

};

class SymbolNode : public Node {
public:
    SymbolNode(ReturnType return_type, size_t id, int line) : Node(NODE_ID, return_type, line), id(id) {}

    void accept(AbstractSyntaxTreeVisitor* vis) const override { vis->visit(*this); }

    [[nodiscard]] size_t get_id() const { return id; }
private:
    size_t id;
};

class EmptyNode : public Node {
public:
    explicit EmptyNode(int line) : Node(NODE_EMPTY, RT_VOID, line) {}

    void accept(AbstractSyntaxTreeVisitor* vis) const override { vis->visit(*this); }
};

#endif //BUILD_NODE_H
