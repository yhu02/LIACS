#include "grammarbuilder.h"

#include <sstream>
#include <limits>

std::unique_ptr <SyntaxTree> GrammarBuilder::retrieve_syntaxtree() {
    if (tree != nullptr)
        return std::move(tree);
    throw std::runtime_error("[SyntaxTreeBuilder] No SyntaxTree was built");
}

void GrammarBuilder::build(NodeBuilder *new_root) {
    if (tree != nullptr)
        throw std::runtime_error("[SyntaxTreeBuilder] tree already built");

    // TODO for students: add passes here, if required
    // (Note: you are allowed to leave this "TODO" until you implemented all assignments)

    tree = std::make_unique<SyntaxTree>(new_root->build());
    delete new_root;
}

void GrammarBuilder::open_scope() {
    scope_stack.emplace(scope_stack.begin(), std::unique_ptr<Scope>());
}

void GrammarBuilder::close_scope() {
    scope_stack.erase(scope_stack.begin());
}

DeclarationList *
GrammarBuilder::add_declaration(DeclarationList *list, const std::string &name, int lineno) {
    if (findSymbol(name) != std::numeric_limits<size_t>::max()) {
        throw std::runtime_error("[GrammarBuilder::add_declaration] line " + std::to_string(lineno)
                                 + ", variable " + name + " is already declared");
    }
    list->addName(name);
    return list;
}

void GrammarBuilder::process(DeclarationList *list, ReturnType rt, int line) {
    size_t start = table.addSymbols(list->begin(rt, line), list->end(rt, line));
    scope_stack.back()->addSymbols(list->begin(start), list->end());
}

NodeBuilder *GrammarBuilder::visit_number(const std::string &value, int lineno) {
    long val = strtol(value.c_str(), nullptr, 10);
    if (val < std::numeric_limits<uint8_t>::min()) {
        logger.error(lineno) << value << " is smaller than minimum value (" << +std::numeric_limits<uint8_t>::min() << ")\n";
        return (new ConstantNodeBuilder(-1))->setReturnType(RT_ERROR);
    }
    if (val > getMax()) {
        logger.error(lineno) << value << " is larger than maximum value (" << +std::numeric_limits<unsigned >::max() << ")\n";
        return (new ConstantNodeBuilder(-1))->setReturnType(RT_ERROR);
    }
    return (new ConstantNodeBuilder((uint64_t) val))->setLineNo(lineno);
}

NodeBuilder *GrammarBuilder::visit_identifier(const std::string &name, int lineno) {
    size_t id = findSymbol(name);
    if (id == std::numeric_limits<size_t>::max()) {
        throw std::runtime_error("[GrammarBuilder:visit_identifier] line " + std::to_string(lineno) +
                                 ", variable " + name + " could not be found!");
    }
    Symbol *symbol = table.getSymbol(id)->get();
    NodeBuilder *symbolNode = new SymbolNodeBuilder(id);
    symbolNode->setNodeType(NODE_ID);
    symbolNode->setReturnType(symbol->getReturnType());
    symbolNode->setLineNo(lineno);
    return symbolNode;
}

NodeBuilder *GrammarBuilder::visit_unary(NodeType nt, NodeBuilder *child, int lineno) {
    NodeBuilder *unaryNode = new UnaryNodeBuilder(std::unique_ptr<NodeBuilder>(child));
    unaryNode->setNodeType(nt);
    unaryNode->setReturnType(child->returnType);
    unaryNode->setLineNo(lineno);
    return unaryNode;
}

NodeBuilder *GrammarBuilder::visit_binary(NodeType nt, NodeBuilder *left_child, NodeBuilder *right_child, int lineno) {
    NodeBuilder *binaryNode = new BinaryNodeBuilder(std::unique_ptr<NodeBuilder>(left_child),
                                                    std::unique_ptr<NodeBuilder>(right_child));
    binaryNode->setNodeType(nt);
    binaryNode->setReturnType(left_child->returnType);
    binaryNode->setLineNo(lineno);
    return binaryNode;
}

NodeBuilder *
GrammarBuilder::visit_ternary(NodeType nt, NodeBuilder *left_child, NodeBuilder *middle_child, NodeBuilder *right_child, int lineno) {
    NodeBuilder *ternaryNode = new TernaryNodeBuilder(std::unique_ptr<NodeBuilder>(left_child),
                                                      std::unique_ptr<NodeBuilder>(middle_child),
                                                      std::unique_ptr<NodeBuilder>(right_child));
    ternaryNode->setNodeType(nt);
    ternaryNode->setLineNo(lineno);
    return ternaryNode;
}

NodeBuilder *GrammarBuilder::visit_empty(int lineno) {
    return (new EmptyNodeBuilder())->setLineNo(lineno);
}

size_t GrammarBuilder::findSymbol(const std::string &name) {
    for (auto it = scope_stack.rbegin(); it != scope_stack.rend(); ++it) {
        if (it->get()->exists(name)) {
            return it->get()->getSymbol(name);
        }
    }
    return std::numeric_limits<size_t>::max();
}
