/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/


#ifndef BUILD_GRAMMARBUILDER_H
#define BUILD_GRAMMARBUILDER_H

#include <syntaxtree.h>
#include <symboltable.h>
#include <logger.h>
#include <src/main/cpp/grammarbuilder/nodebuilder.h>
#include <vector>
#include "scope.h"
#include "decllist.h"

class GrammarBuilder {
public:
    /**
     * Contructs a GrammarVisitor
     * @param logger to log messages to
     */
    explicit GrammarBuilder(Logger& logger, SymbolTable& table) : logger(logger), table(table) {
        scope_stack.push_back(std::make_unique<Scope>());
    };

    /**
     * @return a SyntaxTree based on the current root Node.
     * If the tree is not set, throws an std::runtime_error
     */
    std::unique_ptr<SyntaxTree> retrieve_syntaxtree();

    /**
     * Builds a SyntaxTree
     * Throws an std::runtime_error if another SyntaxTree was already built
     * NOTE: any passes will be performed here (see AbstractPass)
     * @param new_root the Node* to assign as root
     */
    void build(NodeBuilder* new_root);

    /**
     * Opens a new Scope to process variables
     */
    void open_scope();

    /**
     * Closes the current Scope and saves it to the SymbolTable
     */
    void close_scope();

    /**
     * Add a new Declaration to the DeclarationList
     * @param list list to add to
     * @param name name to add
     * @return the DeclarationList
     */
    DeclarationList* add_declaration(DeclarationList* list, const std::string& name, int lineno);

    /**
     * Process a DeclarationList by adding it to a Scope, and the SymbolTable
     * @param list list to process
     */
    void process(DeclarationList* list, ReturnType rt, int lineno);

    /**
     * Constructs a ConstantNode with supplied value
     * Checks if the value is in range of an uint8_t
     * If the value is not in range, logs an error and returns an ErrorNode
     * @param value string representing the value to assign to the Node
     * @param lineno the current line-number (used for logging)
     * @return NodeBuilder* to a new ConstantNodeBuilder containing the supplied value
     */
    NodeBuilder* visit_number(const std::string& value, int lineno);


    /**
     * Constructs a SymbolNode with the corresponding identifier
     * @param name the name of the symbol, used to find the identifier
     * @param lineno the current line-number
     * @return NodeBuilder* to a new SymbolNodeBuilder
     */
    NodeBuilder* visit_identifier(const std::string& name, int lineno);


    /**
     * Constructs an UnaryNodeBuilder
     * @param nt the NodeType of the Node
     * @param child the child Node
     * @param lineno the current line-number (used for logging)
     * @return NodeBuilder* to a new UnaryNodeBuilder
     */
    static NodeBuilder* visit_unary(NodeType nt, NodeBuilder* child, int lineno);

    /**
     * Constructs a BinaryNodeBuilder
     * @param nt the NodeType of the Node
     * @param left_child the left-child of the Node
     * @param right_child the right-child of the Node
     * @param lineno the current line-number (used for logging)
     * @return NodeBuilder* to a new BinaryNodeBuilder
     */
    static NodeBuilder* visit_binary(NodeType nt, NodeBuilder* left_child, NodeBuilder* right_child, int lineno);

    /**
     * Constructs a TernaryNodeBuilder
     * @param nt the NodeType of the node
     * @param left_child the left-child of the Node
     * @param middle_child the middle-child of the Node
     * @param right_child the right-child of the Node
     * @param lineno the current line-number (used for logging)
     * @return NodeBuilder* to a new TernaryNodeBuilder
     */
    static NodeBuilder* visit_ternary(NodeType nt, NodeBuilder* left_child, NodeBuilder* middle_child, NodeBuilder* right_child, int lineno);

    /**
     * Construct an EmptyNodeBuilder
     * @param lineno
     * @return
     */
    static NodeBuilder* visit_empty(int lineno);

    Logger& logger;
private:

    /**
     * Finds the Symbol with the given name in the scope-stack
     * @param name the name of the Symbol to find
     * @return the identifier of the symbol or std::numeric_limits<size_t>::max() if the symbol could not be found
     */
    size_t findSymbol(const std::string& name);

    std::unique_ptr<SyntaxTree> tree = nullptr;
    SymbolTable& table;

    /**
     * Vector representing a stack to keep track of the scopes
     * The first element in the vector is the most outer-scope,
     * the last element in this vector is the most inner-scope
     */
    std::vector<std::unique_ptr<Scope>> scope_stack = {};

    /**
     * @return Maximum number allowed in a program
     */
    unsigned getMax() {
        if (CURRENT_ASSIGNMENT < ASS_THREE)
            return std::numeric_limits<uint8_t>::max();
        return std::numeric_limits<unsigned>::max();
    }
};

#endif //BUILD_GRAMMARBUILDER_H
