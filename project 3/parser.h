/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#define ALLOC(t) (t*) calloc(1, sizeof(t))

#define EXPR      100
#define PRIMARY   101
#define FLAGOP    102

int analysis_body(struct bodyNode* body);

struct ParseInputNode{
    struct programNode* program;
};

struct type_nameNode{
   int type;// INT REAL BOOLEAN STRING LONG 
   char* id; //
};
struct conditionNode
{
    int relop;
    int ln;
    struct primaryNode* left_operand;
    struct primaryNode* right_operand;
};

struct while_stmtNode
{
    int ln; // Line number
    int type;
    struct conditionNode* condition;
    struct bodyNode* body;
    
};

struct assign_stmtNode
{
    char* id;
    int ln;
    struct exprNode* expr;
};

struct exprNode
{
    int op; // PLUS , MINUS, MULT, ... or NOOP
    int tag; // PRIMARY or EXPR
    int ln; // LINE NUMBER
    struct primaryNode* primary;
    struct exprNode * leftOperand;
    struct exprNode * rightOperand;
};

struct primaryNode
{
    int tag; // NUM, REALNUM or ID
    int ln;
    int ival;
    float fval;
    char *id;
};

struct caseNode
{
    int num;
    struct bodyNode* body;	// body of the case
};

struct case_listNode
{
    struct caseNode* cas;	// case is a keyword in C/C++
    struct case_listNode* case_list;
};

struct switch_stmtNode
{
    char* id;
    int ln;
    struct case_listNode* case_list;
};

struct symbol
{
    char* id;
    int type_number;
    int where; // 0 for built-in, 1 for explicit, 2 for implicit
    int flag; // 0 for TYPE, 1 for VAR
    int ln; // counts current line number 
};
struct stmtNode{
   int stmtType; // WHILE, ASSIGN, DO, or SWITCH

    union
    {
        struct while_stmtNode* while_stmt;
        struct assign_stmtNode* assign_stmt;
        struct switch_stmtNode* switch_stmt;
        struct do_stmtNode* do_stmt;
    };


};

struct type_declNode
{
    struct id_listNode* id_list;
    struct type_nameNode* type_name;
};

struct var_declNode
{
    struct id_listNode* id_list;
    struct type_nameNode* type_name;
};


struct stmt_listNode {
  struct stmtNode* stmt;
  struct stmt_listNode* stmt_list;

};

struct bodyNode{
   struct stmt_listNode* stmt_list;
};

struct id_listNode{
   char* id;
   int ln;
   struct type_nameNode type_name;
   struct id_listNode* id_list;
};



struct var_decl_listNode{
  struct var_declNode* var_decl;
  struct var_decl_listNode* var_decl_list;
};


struct var_decl_sectionNode{
  struct var_decl_listNode* var_decl_list;

};

struct type_decl_listNode{
   struct type_declNode* type_decl;
   struct type_decl_listNode* type_decl_list;
};

struct type_decl_sectionNode{
   //struct type_declNode* type_decl;   //TYPE
   struct type_decl_listNode* type_decl_list;
};

struct declNode{
   struct type_decl_sectionNode* type_decl_section;
   struct var_decl_sectionNode* var_decl_section;
};

struct programNode{
    struct declNode* decl;
    struct bodyNode* body;
};
class Parser {
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    
    Token expect(TokenType expected_type);
    Token peek();
    /*
    void parse_program();
    void parse_decl();
    void parse_type_decl_section();
    void parse_type_decl_list();
    void parse_type_decl();
    void parse_type_name();
    void parse_var_decl_section();
    void parse_var_decl_list();
    void parse_var_decl();
    void parse_id_list();
    void parse_body();
    void parse_stmt_list();
    void parse_stmt();
    void parse_assign_stmt();
    void parse_while_stmt();
    void parse_do_stmt();
    void parse_switch_stmt();
    void parse_case_list();
    void parse_case();
    void parse_expr();
    void parse_term();
    void parse_factor();
    void parse_condition();
    void parse_primary();
    */
    void parse_relop();
    
    struct programNode* parse_program();
    struct declNode* parse_decl();
    struct type_decl_sectionNode* parse_type_decl_section();
    struct type_decl_listNode* parse_type_decl_list();
    struct type_declNode* parse_type_decl();
    struct type_nameNode* parse_type_name();
    struct var_decl_sectionNode* parse_var_decl_section();
    struct var_decl_listNode* parse_var_decl_list();
    struct var_declNode* parse_var_decl();
    struct id_listNode* parse_id_list();
    struct bodyNode* parse_body();
    struct stmt_listNode* parse_stmt_list();
    struct stmtNode* parse_stmt();
    struct assign_stmtNode* parse_assign_stmt();
    struct while_stmtNode* parse_while_stmt();
    struct while_stmtNode* parse_do_stmt();
    struct switch_stmtNode* parse_switch_stmt();
    struct case_listNode* parse_case_list();
    struct caseNode* parse_case();
    struct exprNode* parse_expr();
    //struct termNode* parse_term();
    struct exprNode* parse_term();
    //struct factorNode* parse_factor();
    struct exprNode* parse_factor();
    struct conditionNode* parse_condition();
    struct primaryNode* parse_primary();
    //struct relopNode* relop();
    //struct declode* parse_decl();
    
    
  public:
    //void ParseInput();
    struct programNode* ParseInput();
    void initial_symbol_table();
    void check_types();
    int analysis_expr(struct exprNode* expr);
    int unify(int first, int second);
    int analysis_prim(struct  primaryNode* prim);
    int analysis_assign(struct assign_stmtNode* assign_stmt);
    int analysis_while(struct while_stmtNode* whil);
    int analysis_switch(struct switch_stmtNode* swi);
    int analysis_body(struct bodyNode* body);
};

#endif

