/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/


#ifndef BUILD_NODEBUILDER_H
#define BUILD_NODEBUILDER_H

#include "../passes/abstractpass.h"
#include <node.h>


/**
 * This file contains classes to build up several types of Nodes
 * Mandatory arguments are passed through the constructors
 * Through the 'setter'-functions owners can optionally change other member variables
 * Additionally, one can create AbstractPasses to check and modify nodes based on certain conditions
 * To use an AbstractPass, one can use `node.accept(pass)`
 * Finally, to create a node, one can use `node.build()`
 */


class NodeBuilder {
public:
    NodeBuilder() = default;
    virtual ~NodeBuilder() = default;

    virtual void accept(AbstractPass* pass) = 0;
    virtual std::unique_ptr<Node> build() = 0;

    NodeBuilder* setNodeType(NodeType new_type) { nodeType = new_type; return this; }
    NodeBuilder* setReturnType(ReturnType new_type) { returnType = new_type; return this; }
    NodeBuilder* setLineNo(int i) { line = i; return this; }

    NodeType nodeType = NODE_UNKNOWN;
    ReturnType returnType = (CURRENT_ASSIGNMENT & ASS_ONE) ? RT_INT : RT_UNKNOWN;
    int line = 1;
};

class UnaryNodeBuilder : public NodeBuilder {
public:
    explicit UnaryNodeBuilder(std::unique_ptr<NodeBuilder> child) : child(std::move(child)) {}

    void accept(AbstractPass* pass) override { pass->process(*this); }

    std::unique_ptr<Node> build() override {
        return std::make_unique<UnaryNode>( nodeType, returnType, child->build(), line);
    };

    std::unique_ptr<NodeBuilder> child;
};

class BinaryNodeBuilder : public NodeBuilder {
public:
    BinaryNodeBuilder(std::unique_ptr<NodeBuilder> left_child, std::unique_ptr<NodeBuilder> right_child) :
            left_child(std::move(left_child)), right_child(std::move(right_child)) {}

    void accept(AbstractPass* pass) override { pass->process(*this); }

    std::unique_ptr<Node> build() override {
        return std::make_unique<BinaryNode>(nodeType, returnType, left_child->build(), right_child->build(), line);
    };

    std::unique_ptr<NodeBuilder> left_child;
    std::unique_ptr<NodeBuilder> right_child;
};

class TernaryNodeBuilder : public NodeBuilder {
public:
    explicit TernaryNodeBuilder(std::unique_ptr<NodeBuilder> left_child, std::unique_ptr<NodeBuilder> middle_child, std::unique_ptr<NodeBuilder> right_child) :
            left_child(std::move(left_child)), middle_child(std::move(middle_child)), right_child(std::move(right_child)) {};

    void accept(AbstractPass* pass) override { pass->process(*this); }

    std::unique_ptr<Node> build() override {
        return std::make_unique<TernaryNode>(nodeType, returnType, left_child->build(), middle_child->build(), right_child->build(), line);
    };

    std::unique_ptr<NodeBuilder> left_child;
    std::unique_ptr<NodeBuilder> middle_child;
    std::unique_ptr<NodeBuilder> right_child;
};

class ConstantNodeBuilder : public NodeBuilder {
public:
    explicit ConstantNodeBuilder(uint64_t value) : value(value) { nodeType = NODE_NUM; };

    std::unique_ptr<Node> build() override {
        return std::make_unique<ConstantNode>(returnType, value, line);
    }

    void accept(AbstractPass* pass) override { pass->process(*this); }

    uint64_t value;
};

class SymbolNodeBuilder : public NodeBuilder {
public:
    explicit SymbolNodeBuilder(size_t id) : id(id) {};

    std::unique_ptr<Node> build() override {
        return std::make_unique<SymbolNode>(returnType, id, line);
    }

    void accept(AbstractPass* pass) override { pass->process(*this); }

    size_t id;
};

class EmptyNodeBuilder : public NodeBuilder {
public:
    explicit EmptyNodeBuilder() = default;

    std::unique_ptr<Node> build() override {
        return std::make_unique<EmptyNode>(line);
    }

    void accept(AbstractPass* pass) override { pass->process(*this); }

};

#endif //BUILD_NODEBUILDER_H
