%code requires {
    class GrammarBuilder;
    #include "syntaxtree.h"
    class NodeBuilder;
    class DeclarationList;
}

%{

#include <iostream>
#include <cstring>

#include "../cpp/grammarbuilder/grammarbuilder.h"

/* Import from comp.l*/
#ifdef __cplusplus
extern "C" {
#endif

int yylex();   /* Lexer function*/
int yywrap(void);

#ifdef __cplusplus
}
#endif

extern int lineno;

#ifdef DEBUG
int msglevel = 100; /* higher = more debugging messages*/
#else
int msglevel = 0;   /* don't bother sensitive souls*/
#endif


static void yyerror(GrammarBuilder&, const char*);
static std::string cvt_str(char* s);

%}

/* Provides more useful error messages */
%define parse.error verbose

/* Types to pass between lexer, rules and actions. */
%union {
        char* number;
        char* idStr;
        NodeBuilder* node;
        NodeType nt;
        ReturnType rt;
        DeclarationList* list;
}

/* different starts for each assignment */
%token START_EXPRESSION START_MAIN_SINGLE START_MAIN
/* Placeholder for empty-rules */
%token EMPTY
/* Tokens */
%token MAIN ASSIGN
/* numbers */
%token NUMBER
/* variables and types */
%token ID INT INT8 UNSIGNED UINT8
/* statements */
%token IF ELSE PRINT SEMI COMMA WHILE
/* relational operators */
%token EQ NEQ GT GEQ LT LEQ
/* operators */
%token MINUS PLUS TIMES DIV MOD
/* scope */
%token LPAREN RPAREN LBRACE RBRACE
/* endfile */
%token ENDFILE 0


%start program
%parse-param {GrammarBuilder& builder}

%%
// ---- switch for which start-symbol to use in the current assignment ----
program               : START_EXPRESSION expression
                      | START_MAIN_SINGLE main_single
                      | START_MAIN main;

// ---- Assignment-specific program-definitions: set root-node ----
// TODO: Assignment 1. Implement additive expressions
// 
expression	      : ASSIGN additive_expression {
                        NodeBuilder* node = builder.visit_unary(NODE_PRINT, $<node>2, lineno);
                        builder.build(node);
                      }

// TODO: Assignment 2
main_single           : MAIN LBRACE statement RBRACE {
                        NodeBuilder* node = $<node>3;
                        builder.build(node);
                      };

// TODO: Assignment 3 & Assignment 4 & Assignment 5
main                  : MAIN LBRACE expand_infinite RBRACE {
                        NodeBuilder* node = $<node>3;
                        builder.build(node);
                      };

expand_infinite       : variable_definition expand_infinite {
                        $<node>$ = $<node>2;
                      }
                      | scope_block expand_infinite {
                        $<node>$ = builder.visit_binary(NODE_LIST, $<node>1, $<node>2, lineno);
                      }
                      | ass3_statement expand_infinite {
                        $<node>$ = builder.visit_binary(NODE_LIST, $<node>1, $<node>2, lineno);
                      }
                      | ass3_scope_stmt expand_infinite {
                        $<node>$ = builder.visit_binary(NODE_LIST, $<node>1, $<node>2, lineno);
                      }
                      | print_statement {
                        $<node>$ = $<node>1;
                      };

// Assignment 3
ass3_statement        : IF LPAREN comparison_expression RPAREN ass3_statement {
                        $<node>$ = builder.visit_binary(NODE_IF, $<node>3, $<node>5, lineno);
                      }
                      | IF LPAREN comparison_expression RPAREN ass3_statement ELSE ass3_statement {
                        $<node>$ = builder.visit_ternary(NODE_IF_ELSE, $<node>3, $<node>5, $<node>7, lineno);
                      }
                      | WHILE LPAREN comparison_expression RPAREN ass3_statement {
                        $<node>$ = builder.visit_binary(NODE_WHILE, $<node>3, $<node>5, lineno);
                      }
                      | variable_assignment {
                        $<node>$ = $<node>1;
                      };

ass3_scope_stmt       : IF LPAREN comparison_expression RPAREN scope_block {
                        $<node>$ = builder.visit_binary(NODE_IF, $<node>3, $<node>5, lineno);
                      }
                      | IF LPAREN comparison_expression RPAREN scope_block ELSE scope_block {
                        $<node>$ = builder.visit_ternary(NODE_IF_ELSE, $<node>3, $<node>5, $<node>7, lineno);
                      }
                      | IF LPAREN comparison_expression RPAREN ass3_statement {
                        $<node>$ = builder.visit_binary(NODE_IF, $<node>3, $<node>5, lineno);
                      }
                      | IF LPAREN comparison_expression RPAREN scope_block ELSE ass3_statement {
                        $<node>$ = builder.visit_ternary(NODE_IF_ELSE, $<node>3, $<node>5, $<node>7, lineno);
                      }
                      | WHILE LPAREN comparison_expression RPAREN scope_block {
                        $<node>$ = builder.visit_binary(NODE_WHILE, $<node>3, $<node>5, lineno);
                      }
                      | WHILE LPAREN comparison_expression RPAREN ass3_statement {
                        $<node>$ = builder.visit_binary(NODE_WHILE, $<node>3, $<node>5, lineno);
                      };

scope_block           : LBRACE expand_scope {
                        builder.close_scope();
                        $<node>$ = $<node>2;
                      };

expand_scope          : variable_definition expand_scope {
                        $<node>$ = $<node>2;
                      }
                      | variable_definition RBRACE {
                        builder.open_scope();
                      }
                      | variable_assignment expand_scope {
                        $<node>$ = builder.visit_binary(NODE_LIST, $<node>1, $<node>2, lineno);
                      }
                      | variable_assignment RBRACE {
                        builder.open_scope();
                        $<node>$ = $<node>1;
                      }
                      | ass3_scope_stmt expand_scope {
                        $<node>$ = builder.visit_binary(NODE_LIST, $<node>1, $<node>2, lineno);
                      }
                      | ass3_scope_stmt RBRACE {
                        builder.open_scope();
                        $<node>$ = $<node>1;
                      }
                      | scope_block expand_scope {
                        $<node>$ = builder.visit_binary(NODE_LIST, $<node>1, $<node>2, lineno);
                      }
                      | scope_block RBRACE {
                        builder.open_scope();
                        $<node>$ = $<node>1;
                      };

variable_assignment   : ID ASSIGN additive_expression SEMI {
                        $<node>$ = builder.visit_identifier(cvt_str($<idStr>1), lineno);
                        $<node>$ = builder.visit_binary(NODE_ASSIGNMENT, $<node>$, $<node>3, lineno);
                      };

variable_definition   : INT identifiers {
                        builder.process($<list>2, RT_INT, lineno);
                      }
                      | INT8 identifiers {
                        builder.process($<list>2, RT_INT8, lineno);
                      }
                      | UNSIGNED identifiers {
                        builder.process($<list>2, RT_UINT, lineno);
                      }
                      | UNSIGNED INT identifiers {
                        builder.process($<list>3, RT_UINT, lineno);
                      }
                      | UINT8 identifiers {
                        builder.process($<list>2, RT_UINT8, lineno);
                      };

identifiers           : ID SEMI {
                        $<list>$ = new DeclarationList();
                        $<list>$ = builder.add_declaration($<list>$, cvt_str($<idStr>1), lineno);
                      }
                      | ID COMMA identifiers {
                        $<list>$ = builder.add_declaration($<list>3, cvt_str($<idStr>1), lineno);
                      };

// Assignment 2
if_statement          : IF LPAREN comparison_expression RPAREN statement {
                        $<node>$ = builder.visit_binary(NODE_IF, $<node>3, $<node>5, lineno);
                      }
                      | IF LPAREN comparison_expression RPAREN statement ELSE statement {
                        $<node>$ = builder.visit_ternary(NODE_IF_ELSE, $<node>3, $<node>5, $<node>7, lineno);
                      };

statement             : if_statement {
                        $<node>$ = $<node>1;
                      }
                      | print_statement {
                        $<node>$ = $<node>1;
                      }

comparison_expression : additive_expression EQ additive_expression {
                        $<node>$ = builder.visit_binary(NODE_EQ, $<node>1, $<node>3, lineno);
                      }
                      | additive_expression NEQ additive_expression {
                        $<node>$ = builder.visit_binary(NODE_NEQ, $<node>1, $<node>3, lineno);
                      }
                      | additive_expression GT additive_expression {
                        $<node>$ = builder.visit_binary(NODE_GT, $<node>1, $<node>3, lineno);
                      }
                      | additive_expression GEQ additive_expression {
                        $<node>$ = builder.visit_binary(NODE_GEQ, $<node>1, $<node>3, lineno);
                      }
                      | additive_expression LT additive_expression {
                        $<node>$ = builder.visit_binary(NODE_LT, $<node>1, $<node>3, lineno);
                      }
                      | additive_expression LEQ additive_expression {
                        $<node>$ = builder.visit_binary(NODE_LEQ, $<node>1, $<node>3, lineno);
                      };

// Assignment 1
additive_expression   : additive_expression addop term {
                        $<node>$ = builder.visit_binary($<nt>2, $<node>1, $<node>3, lineno);
                      } | term {
                        $<node>$ = $<node>1;
                      };
term                  : term termop factor {
                        $<node>$ = builder.visit_binary($<nt>2, $<node>1, $<node>3, lineno);
                      } | factor {
                        $<node>$ = $<node>1;
                      };
factor                : LPAREN additive_expression RPAREN {
                        $<node>$ = $<node>2;
                      } | unary factor {
                        $<node>$ = builder.visit_unary($<nt>1, $<node>2, lineno);
                      } | NUMBER {
                        $<node>$ = builder.visit_number(cvt_str($<number>1), lineno);
                      } | ID {
                        $<node>$ = builder.visit_identifier(cvt_str($<idStr>1), lineno);
                      };

termop                : TIMES {
			            $<nt>$ = NODE_MUL;
                      } | DIV {
                      	$<nt>$ = NODE_DIV;
                      } | MOD {
                      	$<nt>$ = NODE_MOD;
                      };
addop                 : PLUS {
                        $<nt>$ = NODE_ADD;
                      } | MINUS {
                        $<nt>$ = NODE_SUB;
                      };
unary                 : PLUS {
                        $<nt>$ = NODE_SIGNPLUS;
                      } | MINUS {
                        $<nt>$ = NODE_SIGNMINUS;
                      };

print_statement       : PRINT additive_expression SEMI {
                        $<node>$ = builder.visit_unary(NODE_PRINT, $<node>2, lineno);
                      };
%%

static void yyerror(GrammarBuilder& vis, const char* s) {
    (&vis)->logger.error(lineno) << s << '\n';
}

static std::string cvt_str(char* s) {
    std::string str(s);
    free(s);
    return str;
}

int yywrap() {
    return 1;
}