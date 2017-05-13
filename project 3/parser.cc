/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"
#include <ctype.h>
#include <string.h>
using namespace std;



struct symbol* symbol_table[100]; 
int t_counter;


void assign_primary(struct primaryNode* primary, int type)
{
    int temp = type;
    if(primary->tag == ID)
    {
       char* current_id = primary-> id;
       int i = 0;
       int Found = 0;
       while( i < ( 5 + t_counter) && Found == 0)
       {
         int temp2;
         if(strcmp(current_id, symbol_table[i]->id) == 0)
         {
           temp2 = symbol_table[i] -> type_number;
           for(int j = 0; j < (5 + t_counter); j++)
           {
             if(symbol_table[j]->type_number == temp2)
             {
                symbol_table[j]->type_number = temp;
             }
           }
           Found = 1;
          }
          i++;
        }
     }    

}

void assign_expr(struct exprNode* expr, int type)
{
   int temp = type;
   if(expr -> tag == PRIMARY)
   {
     assign_primary(expr -> primary,temp);
   }
   else if(expr -> tag = EXPR)
   {
     assign_expr(expr->leftOperand, temp);
     assign_expr(expr->rightOperand, temp);
   }

}


/* 
struct type_nameNode* type_name{
   int type;// INT REAL BOOLEAN STRING LONG 
   char id; //
};
struct conditionNode
{
    int relop;
    struct primaryNode* left_operand;
    struct primaryNode* right_operand;
};

struct while_stmtNode
{
    struct conditionNode* condition;
    struct bodyNode* body;
};

struct assign_stmtNode
{
    char* id;
    struct exprNode* expr;
};

struct exprNode
{
    int op; // PLUS , MINUS, MULT, ... or NOOP
    int tag; // PRIMARY or EXPR
    struct primaryNode* primary;
    struct exprNode * leftOperand;
    struct exprNode * rightOperand;
};

struct primaryNode
{
    int tag; // NUM, REALNUM or ID
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
    struct case_listNode* case_list;
};

struct symbol
{
    char* id;
    int type_number;
    int where; // 0 for built-in, 1 for explicit, 2 for implicit
    int flag; // 0 for TYPE, 1 for VAR
};
struct stmtNode stmt{
   int stmtType; // WHILE, ASSIGN, DO, or SWITCH

    union
    {
        struct while_stmtNode* while_stmt;
        struct assign_stmtNode* assign_stmt;
        struct switch_stmtNode* switch_stmt;
        struct do_stmtNode* do_stmt;
    };


};


struct stmt_listNode stmt_list{
  struct stmtNode* stmt;
  struct stmt_listNode stmt_list;

};

struct bodyNode body{
   struct stmt_listNode* stmt_list;
};

struct id_listNode id_list{
   struct type_nameNode type_name;
   struct id_listNode* id_list;
};

struct var_declNode var_decl{
   struct id_listNode* id_list;
   struct type_nameNode type_name;

};

struct var_decl_listNode var_decl_listNode{
  struct var_declNode* var_decl;
  struct var_decl_listNode* var_decl_list;
};


struct var_decl_sectionNode var_decl_section{
  struct var_decl_listNode* var_decl_list;

};

struct type_decl_listNode type_del_list{
   struct type_declNode* type_decl;
   struct type_decl_listNode* type_decl_list;
};

struct type_decl_sectionNode type_decl_section{
   //struct type_declNode* type_decl;   //TYPE
   struct type_decl_listNode* type_decl_list;
};

struct declNode decl{
   struct type_decl_sectionNode* type_decl_section;
   struct var_decl_sectionNode* var_decl_section;
};

struct programNode{
    struct declNode* decl;
    struct bodyNode* body;
};
*/



void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

/*
//void Parser::ParseInput()
struct ParseInputNode* Parser::ParseInput()
{
    struct ParseInputNode* parseinput;
    parseinput = ALLOC(struct ParseInputNode);
    parseinput -> program = program();
    expect(END_OF_FILE);
    return parseinput;
}

*/
void Parser::parse_relop()
//struct relopNode Parser::relop()
{
    // TODO
    
}


//void Parser::parse_expr()
struct exprNode* Parser::parse_expr()
{
  struct exprNode* expr;
  struct exprNode* left_op;
  
  Token t = peek();
  if(t.token_type == LPAREN || t.token_type == ID || t.token_type == NUM || t.token_type == REALNUM)
  {
    left_op = parse_term();
    t = lexer.GetToken();
    if(t.token_type == PLUS)
    {
      expr = ALLOC(struct exprNode);
      expr -> op = t.token_type;
      expr -> leftOperand = left_op;
      expr -> rightOperand = parse_expr();
      expr -> tag = EXPR;
      expr -> ln = t.line_no;
      expr -> primary = NULL;
      return expr;
    }
    else if(t.token_type == RPAREN || t.token_type == MULT|| t.token_type == DIV|| t.token_type == SEMICOLON)
    {
      lexer.UngetToken(t);
      return left_op;
    }
  }
  else
  {
     //cout << " error in parse_expr " << endl;
     syntax_error();
  }
    // TODO
}

//void Parser::fparse_primary()
struct primaryNode* Parser::parse_primary()
{
    // TODO
    struct primaryNode* primary;
    primary = ALLOC(struct primaryNode);
    Token t = lexer.GetToken();
    primary -> ln = t.line_no;
    if(t.token_type == NUM || t.token_type == REALNUM || t.token_type == ID)
    {
      switch(t.token_type)
      {
       case NUM: 
       
          primary -> tag = NUM;
          primary -> ival = atoi((t.lexeme).c_str());
          break;
          
       case REALNUM:
       
          primary -> tag = REALNUM;
          primary -> fval = atof((t.lexeme).c_str());
          break;
       case ID:
       
          primary -> tag = ID;
          primary -> id = strdup((t.lexeme).c_str());
          break;
      
      }
    }
    else
    {
     // cout << "error in parse_primary " << endl;
      syntax_error();
    }
    return primary;
}

//void Parser::parse_condition()
struct conditionNode* Parser::parse_condition()
{  
    struct conditionNode* condition;
    condition = ALLOC(struct conditionNode);
    Token t = peek();
    condition-> ln = t.line_no;
    if(t.token_type == NUM || t.token_type == REAL || t.token_type == ID)
    {
      condition -> left_operand = parse_primary();
      Token t1 = lexer.GetToken();
      if(t1.token_type == GREATER || t1.token_type == LESS || t1.token_type == NOTEQUAL || t1.token_type == GTEQ || t1.token_type == LTEQ)
      {
        condition -> right_operand = parse_primary();
        condition -> relop = t1.token_type;
        
      }
      else if(t.token_type == ID)
      {
        
        lexer.UngetToken(t1);
        condition->right_operand = NULL;
        condition->relop = FLAGOP;
      }
      else
      {
         syntax_error();
      }
    }
    else
    {
       //cout << " error in parse_condition " << endl;
       syntax_error();
    }
    return condition;
    // TODO
}


//void Parser::parse_factor()
struct exprNode* Parser::parse_factor()
{
   struct exprNode* factor;
   Token t = lexer.GetToken();
   
   if(t.token_type == LPAREN)
   {
      factor = parse_expr();
      expect(RPAREN);
   }
   else if(t.token_type == NUM)
   {
      factor = ALLOC(struct exprNode);
      factor -> ln = t.line_no;
      factor -> primary = ALLOC(struct primaryNode);
      
      factor -> tag = PRIMARY;
      factor -> op = FLAGOP;
      factor -> leftOperand = NULL;
      factor -> rightOperand = NULL;
      factor -> primary -> tag = NUM;
      factor -> primary -> ival = atoi((t.lexeme).c_str());
      
   }
   else if(t.token_type == REALNUM)
   {
    factor = ALLOC(struct exprNode);
    factor -> ln = t.line_no;
    factor -> primary = ALLOC(struct primaryNode);
    factor -> tag = PRIMARY;
    factor -> op = FLAGOP;
    factor -> leftOperand = NULL;
    factor -> rightOperand = NULL;
    factor -> primary -> tag = REALNUM;
    factor -> primary ->fval = atof((t.lexeme).c_str());
   }
   else if(t.token_type == ID)
   {
      factor = ALLOC(struct exprNode);
      factor -> ln = t.line_no;
      factor -> primary = ALLOC(struct primaryNode);
      factor -> tag = PRIMARY;
      factor -> op = FLAGOP;
      factor -> leftOperand = NULL;
      factor -> rightOperand = NULL;
      factor -> primary -> tag = ID;
      factor -> primary -> id = strdup((t.lexeme).c_str());

   }
   else
   {
     //cout << " error in parse_factor" << endl;
     syntax_error();
   }
    // TODO
    return factor;
}

//void Parser::parse_term()
struct exprNode* Parser::parse_term()
{
  struct exprNode* term;
  struct exprNode* left_op;
  Token t = peek();
  if(t.token_type == LPAREN || t.token_type == NUM || t.token_type == REALNUM || t.token_type == ID)
  {
      left_op = parse_factor();
      t = lexer.GetToken();
      if(t.token_type == MULT || t.token_type == DIV)
      {
        term = ALLOC(struct exprNode);
        term -> op = t.token_type;
        term -> ln = t.line_no;
        term -> leftOperand = left_op;
        term -> rightOperand = parse_term();
        term -> primary = NULL;
        term -> tag = EXPR;
        return term;
      }
      else if(t.token_type == RPAREN || t.token_type == PLUS || t.token_type == SEMICOLON)
      {
        lexer.UngetToken(t);
        return left_op;
      }
      else
      {
        //cout<<" error in parse_term 1" << endl;
        syntax_error();
      }
  }
  else
  {
   // cout <<" error in parse_term 2" << endl;
    syntax_error();
  }
    
    // TODO
}


//void Parser::parse_case()
struct caseNode* Parser::parse_case()
{
   struct caseNode* cases;
   cases = ALLOC(struct caseNode);
   expect(CASE);
   cases->num = atoi((expect(NUM).lexeme).c_str());
   expect(COLON);
   cases -> body = parse_body();
   return cases;
    // TODO
}

//void Parser::parse_case_list()
struct case_listNode* Parser::parse_case_list()
{
   struct case_listNode* caseList;
   caseList = ALLOC(struct case_listNode);
   caseList -> cas = parse_case();
   Token t = peek();
   //note *****
   if(t.token_type == CASE)
   {
     caseList -> case_list = parse_case_list();
   }
   else if(t.token_type == RBRACE)
   {
      caseList -> case_list = NULL;
   }
   else
     {
       syntax_error();
     }
    return caseList;
    // TODO
}


//void Parser::parse_assign_stmt()
struct assign_stmtNode* Parser::parse_assign_stmt()
{
    struct assign_stmtNode* assignStmt;
    assignStmt = ALLOC(struct assign_stmtNode);
    Token t = lexer.GetToken();
    assignStmt -> id = strdup(t.lexeme.c_str());
    assignStmt -> ln = t.line_no;
    expect(EQUAL);
    assignStmt -> expr = parse_expr();
    expect(SEMICOLON);
    return assignStmt;
    // TODO
     
}

//void Parser::parse_while_stmt()
struct while_stmtNode* Parser::parse_while_stmt()
{
    struct while_stmtNode* whileStmt;
    Token t = peek();
    //WhileStmt = ALLOC(struct while_StmtNode);
    expect(WHILE);
     whileStmt = ALLOC(struct while_stmtNode);
    whileStmt -> ln = t.line_no;
    whileStmt -> type = 0;
    whileStmt -> condition = parse_condition();
    whileStmt -> body = parse_body();
    return whileStmt;
    // TODO
}

//void Parser::parse_do_stmt()
struct while_stmtNode* Parser::parse_do_stmt()
{
     struct while_stmtNode* doStmt;
     //doStmt = ALLOC(struct while_stmtNode);
     expect(DO);
     
     doStmt = ALLOC(struct while_stmtNode);
     doStmt -> body = parse_body();
     doStmt -> type = 1;
     expect(WHILE);
     doStmt -> condition = parse_condition();
     expect(SEMICOLON);
     return doStmt;
    // TODO
}

//void Parser::parse_switch_stmt()
struct switch_stmtNode* Parser::parse_switch_stmt()
{
    struct switch_stmtNode* switchStmt;
    //switchStmt = ALLOC(struct switch_stmtNode);
    expect(SWITCH);
    switchStmt = ALLOC(struct switch_stmtNode);
    Token t = lexer.GetToken();
    switchStmt -> id = strdup(t.lexeme.c_str());
    switchStmt -> ln = t.line_no;
    expect(LBRACE);
    switchStmt->case_list = parse_case_list();
    expect(RBRACE);
    return switchStmt;
    // TODO
}



//void Parser::parse_stmt()
struct stmtNode* Parser::parse_stmt()
{
    struct stmtNode* stm;
    stm = ALLOC(struct stmtNode);
    // stmt -> assign_stmt
    // stmt -> while_stmt
    // stmt -> do_stmt
    // stmt -> switch_stmt
    Token t = peek();
    if (t.token_type == ID)
    {
        // stmt -> assign_stmt
        stm -> stmtType = 110; // ASSIGN TYPE
       stm -> assign_stmt = parse_assign_stmt();
    }
    else if (t.token_type == WHILE)
    {
        // stmt -> while_stmt
        stm -> stmtType = 111; //WHILE TYPE
       stm -> while_stmt = parse_while_stmt();
    }
    else if (t.token_type == DO)
    {
        // stmt -> do_stmt
        stm -> stmtType = 112; //DO TYPE
       //stm -> do_stmt = parse_do_stmt();
       stm -> while_stmt = parse_do_stmt();
    }
    else if (t.token_type == SWITCH)
    {
       stm -> stmtType = 113; // SWITCH TYPE
        // stmt -> switch_stmt
       stm -> switch_stmt = parse_switch_stmt();
    }
    else
    {
       // cout << " error in parse_stmt " << endl;
        syntax_error();
    }
    return stm;
}

//void Parser::parse_stmt_list()
struct stmt_listNode* Parser::parse_stmt_list()
{
    struct stmt_listNode* stmtList;
    stmtList = ALLOC(struct stmt_listNode);
    
    // stmt_list -> stmt
    // stmt_list -> stmt stmt_list
    stmtList  -> stmt = parse_stmt();
    Token t = peek();
    if (t.token_type == WHILE || t.token_type == ID ||
        t.token_type == SWITCH || t.token_type == DO)
    {
        // stmt_list -> stmt stmt_list
        stmtList -> stmt_list = parse_stmt_list();
    }
    else if (t.token_type == RBRACE)
    {
        // stmt_list -> stmt
        // stmtList -> stmt_list = NULL;
        
    }
    else
    {
        //cout << " error in parse_stmt_list " << endl;
        syntax_error();
    }
    return stmtList;
}

//void Parser::parse_body()
struct bodyNode* Parser::parse_body()
{
    // body -> LBRACE stmt_list RBRACE
    struct bodyNode* bby;
    bby = ALLOC(struct bodyNode);
    
    
    expect(LBRACE);
    bby-> stmt_list = parse_stmt_list();
    expect(RBRACE);
    return bby;
}

//void Parser::parse_id_list()
struct id_listNode* Parser::parse_id_list()
{
    // id_list -> ID
    // id_list -> ID COMMA id_list
    struct id_listNode* idL;
    idL = ALLOC(struct id_listNode);
    idL -> id = strdup(expect(ID).lexeme.c_str());
    Token t = lexer.GetToken();
    idL -> ln = t.line_no;
    if (t.token_type == COMMA)
    {
        // id_list -> ID COMMA id_list
        
       idL-> id_list = parse_id_list();
    }
    else if (t.token_type == COLON)
    {
        // id_list -> ID
        //idL->id_list = NULL;
        lexer.UngetToken(t);
    }
    else
    {
        //cout << " error in parse_id_list " << endl;
        syntax_error();
    }
    return idL;
}

//void Parser::parse_var_decl()
struct var_declNode* Parser::parse_var_decl()
{
    struct var_declNode* varDecl;
    varDecl = ALLOC(struct var_declNode);
    
    // var_decl -> id_list COLON type_name SEMICOLON
    
    varDecl-> id_list =  parse_id_list();
    
    expect(COLON);
    varDecl-> type_name = parse_type_name();
    expect(SEMICOLON);
    return varDecl;
}
//void Parser::parse_var_decl_list()
struct var_decl_listNode* Parser::parse_var_decl_list()
{
    struct var_decl_listNode* varDeclList;
    varDeclList = ALLOC(struct var_decl_listNode);
    
    
    // var_decl_list -> var_decl
    // var_decl_list -> var_decl var_decl_list
    //  parse_var_decl();
    varDeclList-> var_decl = parse_var_decl();
    Token t = peek();
    
    if (t.token_type == LBRACE)
    {
    
        //varDeclList-> var_decl_list = NULL;
        // var_decl_list -> var_decl
    }
    else if (t.token_type == ID)
    {
        // var_decl_list -> var_decl var_decl_list
        
        varDeclList-> var_decl_list = parse_var_decl_list();
    }
    else
    {
        //cout << " error in parse_var_decl_list " << endl;
        syntax_error();
    }
    return varDeclList;
}

//void Parser::parse_var_decl_section()
struct var_decl_sectionNode* Parser::parse_var_decl_section()
{
    // var_decl_section -> VAR var_decl_list
    // var_decl_section -> epsilon
    
    struct var_decl_sectionNode* varDeclSection;
    
    varDeclSection = ALLOC(struct var_decl_sectionNode);
    
    Token t = lexer.GetToken();
    if (t.token_type == VAR)
    {
        
        // var_decl_section -> VAR var_decl_list
        
       varDeclSection -> var_decl_list = parse_var_decl_list();
    }
    else if (t.token_type == LBRACE)
    {
        varDeclSection = NULL;
        // var_decl_section -> epsilon
        lexer.UngetToken(t);
    }
    else
    {
       // cout << " error in the parse_var_decl_section " << endl;
        syntax_error();
       
    }
    return varDeclSection;
}
//void Parser::parse_type_name()
struct type_nameNode* Parser::parse_type_name()
{
    struct type_nameNode* type_nam;
    type_nam = ALLOC(struct type_nameNode);
    
    // type_name -> REAL
    // type_name -> INT
    // type_name -> BOOLEAN
    // type_name -> STRING
    // type_name -> LONG
    // type_name -> ID
    Token t = lexer.GetToken();
    type_nam->id = strdup(t.lexeme.c_str());
    if (t.token_type == REAL)
    {
        type_nam -> type = 0; //REAL
        // type_name -> REAL
    }
    else if (t.token_type == INT)
    {
         type_nam -> type = 1; //INT
        // type_name -> INT
    }
    else if (t.token_type == BOOLEAN)
    {
        type_nam -> type = 2;  //BOOLEAN
        // type_name -> BOOLEAN
    }
    else if (t.token_type == STRING)
    {
        type_nam -> type = 3;  //STRING
        // type_name -> STRING
    }
    else if (t.token_type == LONG)
    {
        type_nam -> type = 4;  //LONG
        // type_name -> LONG
    }
    else if (t.token_type == ID)
    {
        type_nam -> type = 5;  //ID
        type_nam -> id = strdup((t.lexeme).c_str());
        // type_name -> ID
    }
    else
    {
       // cout << "error in parse_type_name " << endl;
        syntax_error();
       
    }
    return type_nam;
}

//void Parser::parse_type_decl()
struct type_declNode* Parser::parse_type_decl()
{
    // type_decl -> id_list COLON type_name SEMICOLON
    struct type_declNode* typeDecl;
    typeDecl = ALLOC(struct type_declNode);
    
    
    typeDecl-> id_list = parse_id_list();
    expect(COLON);
    typeDecl -> type_name = parse_type_name();
    expect(SEMICOLON);
    return typeDecl;
}

//void Parser::parse_type_decl_list()
struct type_decl_listNode* Parser::parse_type_decl_list()
{
    struct type_decl_listNode* typeDeclList;
    typeDeclList = ALLOC(struct type_decl_listNode);
    // type_decl_list -> type_decl
    // type_decl_list -> type_decl type_decl_list
    typeDeclList -> type_decl = parse_type_decl();
    //typeDeclList -> parse_type_decl();
    
    Token t = peek();
    if (t.token_type == LBRACE || t.token_type == VAR)
    {
        // type_decl_list -> type_decl
        //typeDeclList = NULL;
        //return typeDecList;
        
    }
    else if (t.token_type == ID)
    {
        
        // type_decl_list -> type_decl type_decl_list
        typeDeclList -> type_decl_list = parse_type_decl_list();
        //return typeDeclList;
    }
    else
    {
        //cout << " error in parse_type_decl_list " << endl;
        syntax_error();
       
        
    }
    return typeDeclList;
}

//void Parser::parse_type_decl_section()
struct type_decl_sectionNode* Parser::parse_type_decl_section()
{

    struct type_decl_sectionNode* type_dec_sec;
    type_dec_sec = ALLOC(struct type_decl_sectionNode);
    // type_decl_section -> TYPE type_decl_list
    // type_decl_section -> epsilon
    Token t = lexer.GetToken();
    if (t.token_type == TYPE)
    {
        // type_decl_section -> TYPE type_decl_list
        type_dec_sec -> type_decl_list = parse_type_decl_list();
    }
    else if (t.token_type == LBRACE || t.token_type == VAR)
    {
        // type_decl_section -> epsilon
        lexer.UngetToken(t);
    }
    else
    {
       // cout << " error in parse_type_decl_section " << endl;
        syntax_error();
    }
    return type_dec_sec;
}


//void Parser::parse_decl()
struct declNode* Parser::parse_decl()
{
    struct declNode* dec;
    dec = ALLOC(struct declNode);
    
    // decl -> type_decl_section var_decl_section
    //dec = parse_type_decl_section();
    //dec = parse_var_decl_section();
    dec->type_decl_section = parse_type_decl_section();
    dec->var_decl_section = parse_var_decl_section();
    return dec;
}

//void Parser::parse_program()
struct programNode* Parser::parse_program()
{
    // program -> decl body
    struct programNode* prog;
    prog = ALLOC(struct programNode);
   
    //prog = parse_decl();
    //prog = parse_body();
    prog -> decl = parse_decl();
    prog -> body = parse_body();
    
    return prog;
}


//void Parser::ParseInput()
struct programNode* Parser::ParseInput()
{
   struct programNode* program;
    program = parse_program();
    expect(END_OF_FILE);
    return program;
}

void Parser::initial_symbol_table()
{
    t_counter = 0;    //initialize the count of types
    
    
        struct symbol* boolean_type = ALLOC(struct symbol);
    boolean_type->id = "BOOLEAN";
    boolean_type->type_number = 2;
    boolean_type->where = 0;
    boolean_type->flag = 0;
    symbol_table[0] = boolean_type;
    
   struct symbol* int_type = ALLOC(struct symbol);
    int_type->id = "INT";
    int_type->type_number = 0;
    int_type->where = 0;// 0 for build-in types;
    int_type->flag = 0; // 0 for types, 1 for vars
    symbol_table[1] = int_type;
    
      struct symbol* long_type = ALLOC(struct symbol);
    long_type->id = "LONG";
    long_type->type_number = 4;
    long_type->where = 0;
    long_type->flag = 0;
    symbol_table[2] = long_type;
    
    struct symbol* real_type = ALLOC(struct symbol);
    real_type->id = "REAL";
    real_type->type_number = 1;
    real_type->where = 0;
    real_type->flag = 0;
    symbol_table[3] = real_type;

   struct symbol* string_type = ALLOC(struct symbol);
    string_type->id = "STRING";
    string_type->type_number = 3;
    string_type->where = 0;
    string_type->flag = 0;
    symbol_table[4] = string_type;

 
}


int Parser::unify(int first, int second)
{
    if((first < 5) && (second < 5)) // initially in symbol_table;
    {
        if (first == second)
        {
            return first;
        }
        else
            return -1;
    }
    else if((first > 4) && (second > 4)) // user defined  
    {
      if(first <= second)
      {
        return first;
      }
      else
      {
        return second;
      }
    }
    else
    {
        if(first < 5)
        {
            return first;
        }
        else if(second < 5)
        {
            return second;
        }
    }
}



int Parser::analysis_prim(struct primaryNode* prim)
{
    if (prim->tag == ID)
        {
            
            char* current_id = prim->id;
                int i = 0;
                while (i < (5 + t_counter))
                {
                                  
                    if (strcmp(current_id, symbol_table[i]->id) == 0)
                    {
                        if (symbol_table[i]->flag == 0)
                        {
                            cout << "ERROR CODE 1.4 "<< symbol_table[i] ->id << endl;
                            return -1;
                        }
                        else
                        {
                            return symbol_table[i]->type_number;
                            
                        }
                    }
                    ++i;
                }
         
                    ++t_counter;                
                    symbol_table[4+t_counter] = ALLOC(struct symbol);
                    symbol_table[4+t_counter]->id = (char*)malloc(strlen(current_id) * sizeof(char));
                    strcpy(symbol_table[4+t_counter]->id, current_id);
                    symbol_table[4+t_counter]->type_number = (4+t_counter);
                    symbol_table[4+t_counter]->where = 2;
                    symbol_table[4+t_counter]->flag = 1;
                    return (4+t_counter); 
                
        }
        else if (prim->tag == NUM)
            return 0; // INT
        else if (prim->tag == REALNUM)
            return 1; // REAL
}

int Parser::analysis_expr(struct exprNode* expr)
{
    //Parser parser;
    Token t = peek();
    if (expr->tag == EXPR)
    {
        int lhs_number, rhs_number, result;
        lhs_number = analysis_expr(expr->leftOperand);
        rhs_number = analysis_expr(expr->rightOperand);
        if ((lhs_number < 0) || (rhs_number < 0))
        {
            return -1;
        }
        result = unify(lhs_number, rhs_number);
        if (result < 0)
        {
             cout << "TYPE MISMATCH " << expr->ln << " C2" << endl;
        }
        if(lhs_number != rhs_number && lhs_number == result)
        {
             assign_expr(expr->leftOperand,result);
        }
        else if(lhs_number != rhs_number && rhs_number == result)
        {
             assign_expr(expr->rightOperand,result);
        }
        return result;
    }
    else if (expr->tag == PRIMARY)
    {
        return analysis_prim(expr->primary);
    }
}
/*
void assign_primary(struct primaryNode* primary, int type)
{
    int temp = type;
    if(primary->tag == ID)
    {
       char* current_id = primary-> id;
       int i = 0;
       int Found = 0;
       while( i < ( 5 + t_counter) && Found == 0)
       {
         int temp2;
         if(strcmp(current_id, symbol_table[i]->id) == 0)
         {
           temp2 = symbol_table[i] -> type_number;
           for(int j = 0; j < (5 + t_counter); j++)
           {
             if(symbol_table[j]->type_number == temp2)
             {
                symbol_table[j]->type_number = temp;
             }
           }
           Found = 1;
          }
          i++;
        }
     }    

}

void assign_expr(struct exprNode* expr, int type)
{
   int temp = type;
   if(expr -> tag == PRIMARY)
   {
     assign_primary(expr -> primary,temp);
   }
   else if(expr -> tag = EXPR)
   {
     assign_expr(expr->leftOperand, temp);
     assign_expr(expr->rightOperand, temp);
   }

}
*/
int Parser::analysis_assign(struct assign_stmtNode* assign_stmt)
{
    //Token t = peek();
    char* current_id = assign_stmt->id;
                int index = 0;
                int i = 0;
                int flag_found = 0;
               
                int lhs_number, rhs_number, result;
                while ((i < (5 + t_counter)) && (flag_found == 0))
                {
                     
                    if (strcmp(current_id, symbol_table[i]->id) == 0)
                    {
                        if (symbol_table[i]->flag == 0)
                        {
                            cout << "ERROR CODE 1.4 "<< symbol_table[i] ->id << endl;
                            return -1;
                        }
                        else
                        {
                            lhs_number = symbol_table[i]->type_number;
                            flag_found = 1;
                            index = i;
                        }
                    }
                    ++i;
                }
                if(flag_found == 0)
                {
                    ++t_counter;                
                    symbol_table[4+t_counter] = ALLOC(struct symbol);
                    symbol_table[4+t_counter]->id = (char*)malloc(strlen(current_id) * sizeof(char));
                    strcpy(symbol_table[4+t_counter]->id, current_id);
                    
                     symbol_table[4+t_counter]->where = 2;
                    symbol_table[4+t_counter]->flag = 1;
                    symbol_table[4+t_counter]->type_number = (4+t_counter);
                   
                    lhs_number = symbol_table[4+t_counter]->type_number;
                    index = 4+t_counter;
                }
                
     
                rhs_number = analysis_expr(assign_stmt->expr);
     if (rhs_number >= 0)
    {
                result = unify(lhs_number, rhs_number);
                
                if (result < 0 )
                {
                     cout << "TYPE MISMATCH " << assign_stmt->ln << " C1" << endl;
                     return -1;
                }
                else
                {
                   int temp = symbol_table[index]->type_number;
                    int temp2 = 5;
                    while(temp2 < (5+t_counter))
                    {
                     if(symbol_table[temp2]->type_number == temp)
                     {
                        symbol_table[temp2]->type_number = result;
                     }
                     ++temp2;


                   }
                    symbol_table[index]->type_number = result;
                    assign_expr(assign_stmt->expr, result);
                }
               
           if(result > 0)
           {
             return 0;
           }
    }
     return rhs_number;
}

int Parser::analysis_while(struct while_stmtNode* whil)
{
    int body_number;
    if(whil -> type == 1)
    {
    body_number = analysis_body(whil->body);
    
    
    if (body_number < 0)
    {
        return -1;
    }
    }
    if (whil->condition->relop != FLAGOP)
    {
        int lhs_number, rhs_number, result;
        lhs_number = analysis_prim(whil->condition->left_operand);
        rhs_number = analysis_prim(whil->condition->right_operand);
        if(lhs_number < 0 || rhs_number < 0)
        {
               return -1;
        }
        result = unify(lhs_number, rhs_number);
        if (result < 0)
        {
               cout << "TYPE MISMATCH " << whil-> condition ->ln <<" C3" << endl;
               return -1;
        }
        else
        {
           assign_primary(whil->condition->left_operand, result);
           
           
           assign_primary(whil->condition->right_operand, result);
        }
    }
    else
    {
       int check = analysis_prim(whil->condition->left_operand);
       if(check == -1)
       {
          return -1;
       }
       else if(check == 2)
       {
          //note***
          return 2;
       }
       else if(check > 4)
       {
           for(int i = 5; i < (5 + t_counter); i++)
           {
              if(symbol_table[i]->type_number == check)
              {
                symbol_table[i] -> type_number = 2;
              }
             
           }
       }
       else
       {
          cout << "TYPE MISMATCH " << whil->condition->ln << " C4" << endl;
          return -1;
       } 
    }
    if(whil -> type == 0)
    {
    body_number = analysis_body(whil->body);
    
    
    if (body_number < 0)
    {
        return -1;
    }
    }
    return body_number;
}

int Parser::analysis_switch(struct switch_stmtNode* swi)
{
    char* current_id = swi->id;
               
               int i = 0;
               int flag_found = 0;
                
                
  while ((i < (5 + t_counter)) && (flag_found == 0))
   {
                       
       if (strcmp(current_id, symbol_table[i]->id) == 0)
          {
            if( symbol_table[i]->type_number < 5 && symbol_table[i]->type_number !=0 )
            {
                 cout << "TYPE MISMATCH " << swi -> ln <<" C5" <<endl;
                 return -1;
            }
            else if(symbol_table[i] -> flag == 0 )     
            {
                 cout << "ERROR CODE 1.4 " << current_id << endl;
                 return -1; 
            
            }
            else if(symbol_table[i]->type_number >= 5)
            {
              int check = symbol_table[i]-> type_number;
              for(int i = 0; i < (5+t_counter); i++)
              {
                if(symbol_table[i]-> type_number == check)
                {
                   symbol_table[i]->type_number = 0;
                }
              }
              flag_found = 1;
            }  
                    
                
                        
                        
                        else
                        {
                            flag_found = 1;
                        }
          }
                    ++i;
   }
                if(flag_found == 0)
                {
                    ++t_counter;                
                    symbol_table[4+t_counter] = ALLOC(struct symbol);
                    symbol_table[4+t_counter]->id = (char*)malloc(strlen(current_id) * sizeof(char));
                    strcpy(symbol_table[4+t_counter]->id, current_id);
                    symbol_table[4+t_counter]->type_number = 0;
                    symbol_table[4+t_counter]->where = 2;
                    symbol_table[4+t_counter]->flag = 1;
                }

       struct case_listNode* case_list = swi->case_list;
    int case_type;
    while(case_list != NULL)
    {
        struct caseNode* current_case = case_list->cas;
        case_type = analysis_body(current_case->body);
        if (case_type < 0)
        {
            return case_type;
        }   
        case_list = case_list->case_list;
    }
    return case_type;
}

int Parser::analysis_body(struct bodyNode* body)
{
    int i = 0;
    int flag_found = 0;
    int overall_type = 0;

    struct stmt_listNode* program_stmt = body->stmt_list;
    while(program_stmt != NULL)
    {
        struct stmtNode* current_stmt = program_stmt->stmt;
        switch (current_stmt->stmtType)
        {
            case 110:
                overall_type = analysis_assign(current_stmt->assign_stmt);
                break;
            case 111:
                overall_type = analysis_while(current_stmt->while_stmt);
                break;
            case 112:
                overall_type = analysis_while(current_stmt->while_stmt);
                break;
            case 113:
                overall_type = analysis_switch(current_stmt->switch_stmt);
                break;
        } 
        if (overall_type < 0)
        {
            return overall_type;
        }   
        program_stmt = program_stmt->stmt_list;
    }
    return overall_type;
} 


int main()
{
    Parser parser;
    struct programNode* program;
    program = parser.ParseInput();
    parser.initial_symbol_table(); //initialize symbol table
    //parser.check_types();
    
  //checking types**********************************
     if (program -> decl -> type_decl_section != NULL)
 {
        
        struct type_decl_listNode* currentTypeList = program -> decl->type_decl_section->type_decl_list;
        // while not NULL
        while(currentTypeList != NULL)
        {
            struct id_listNode* current_id_list = currentTypeList->type_decl->id_list;
            
            struct type_nameNode* current_type_name = currentTypeList->type_decl->type_name;
           
            while(current_id_list != NULL)
            
   {
   
   
   
       if(strcmp(current_type_name->id, current_id_list->id) == 0)
       
       {
          cout <<"ERROR CODE 1.1 " <<  current_id_list->id <<endl;
          
          
          
          return 1;
       }
         
                int i;
                for (i = 0; i < (5 + t_counter); i++)
                {
                    if (strcmp(current_id_list->id, symbol_table[i]->id) == 0)
                    {
                        if(symbol_table[i]->where == 0 || symbol_table[i] -> where == 1)
                        {
                        cout <<"ERROR CODE 1.1 " <<  current_id_list->id <<endl;
                        }
                        else
                        {
                        cout <<"ERROR CODE 1.2 " <<  current_id_list->id <<endl;
                        }
                        return 1;
                    }
                }
                
                ++t_counter;                
                symbol_table[4+t_counter] = ALLOC(struct symbol);
                
                symbol_table[4+t_counter]->id = (char*)malloc(strlen(current_id_list->id) * sizeof(char));
                 //current_id_list = current_id_list->id_list;
                strcpy(symbol_table[4+t_counter]->id, current_id_list->id);
                
                 symbol_table[4+t_counter]->where = 1;
                symbol_table[4+t_counter]->flag = 0;   
                symbol_table[4+t_counter]->type_number = (4+t_counter);
                             
                current_id_list = current_id_list->id_list;
            }
            
            
            int i = 0;
            int flag_found = 0;
            
            
            int rhs_number = -1;
            
            while ((i < (5 + t_counter)) && (flag_found == 0))
            {
                if (strcmp(current_type_name -> id , symbol_table[i]->id) == 0)
                {
                    rhs_number = symbol_table[i]->type_number;
                    
                    
                    flag_found = 1;
                }
                ++i;
            }
            if(flag_found == 0)
            {
                ++t_counter;                
                
                
                symbol_table[4+t_counter] = ALLOC(struct symbol);
                
                
                symbol_table[4+t_counter]->id = (char*)malloc(strlen(current_type_name->id) * sizeof(char));
                
                
                strcpy(symbol_table[4+t_counter]->id, current_type_name->id);
                 symbol_table[4+t_counter]->where = 2;
                 
                symbol_table[4+t_counter]->flag = 0;
                
                symbol_table[4+t_counter]->type_number = (4+t_counter);
            
                rhs_number = 4+t_counter;
            }           
            current_id_list = currentTypeList->type_decl->id_list;
            
            while(current_id_list != NULL)
            {
              
              for(int i = 5; i< (5 + t_counter); i++)
              {
                  symbol_table[i] -> type_number = rhs_number;
              }
              
               current_id_list = current_id_list->id_list;
            }
    
            currentTypeList = currentTypeList->type_decl_list;
        } 
   
}
      
       //********************checking vars*********************
        if (program ->decl-> var_decl_section != NULL)
    {
        
        struct var_decl_listNode* currentVarList = program -> decl -> var_decl_section -> var_decl_list;
        while(currentVarList != NULL)
        {
            struct id_listNode* current_id_list = currentVarList->var_decl->id_list;
            struct type_nameNode* current_type_name = currentVarList->var_decl->type_name;
 
             while(current_id_list != NULL)
            {
              
                int i;
                for (i = 0; i < (5 + t_counter); i++)
                {   
                    if (strcmp(current_id_list->id, symbol_table[i]->id) == 0)
                    {
                        if (symbol_table[i]->flag == 1)
                        {                      
                            cout << "ERROR CODE 2.1 "<< symbol_table[i] -> id <<endl;
                            return 1;
                        }
                        else if (symbol_table[i]->flag == 0)
                        {
                            
                            cout << "ERROR CODE 1.3 " << symbol_table[i]->id << endl;
                            return 1;
                        
                        }
                    }
                }
                
                ++t_counter;                
                symbol_table[4+t_counter] = ALLOC(struct symbol);
                symbol_table[4+t_counter]->id = (char*)malloc(strlen(current_id_list->id) * sizeof(char));
                 //current_id_list = current_id_list->id_list;
                strcpy(symbol_table[4+t_counter]->id, current_id_list->id);
                symbol_table[4+t_counter]->type_number = (4+t_counter);
                symbol_table[4+t_counter]->where = 1;
                symbol_table[4+t_counter]->flag = 1;                 
                current_id_list = current_id_list->id_list;
                
                
               
            }
            int i = 0 ;
            int flag_found= 0;
           
            int rhs_number = -1;
            
            
            while ((i < (5 + t_counter)) && (flag_found == 0))
            {               
                if (strcmp(current_type_name -> id, symbol_table[i]->id) == 0)
                {
                    if (symbol_table[i]->flag == 1)
                    {
                        cout << "ERROR CODE 2.2 "<< symbol_table[i]->id << endl;
                        return 1;
                    }
                    else
                    {
                        rhs_number = symbol_table[i]->type_number;
                        flag_found = 1;
                    }
                }
                ++i;
            }
            if(flag_found == 0)
            {
                ++t_counter;                
                symbol_table[4+t_counter] = ALLOC(struct symbol);
                symbol_table[4+t_counter]->id = (char*)malloc(strlen(current_type_name->id) * sizeof(char));
                strcpy(symbol_table[4+t_counter]->id, current_type_name->id);
                symbol_table[4+t_counter]->type_number = (4+t_counter);
                symbol_table[4+t_counter]->where = 2;
                symbol_table[4+t_counter]->flag = 0;
                rhs_number = 4+t_counter;
            }           
            current_id_list = currentVarList->var_decl->id_list;
            while(current_id_list != NULL)
            {
              for(int i = 0; i < (5+t_counter); i++)
              {
               if(strcmp((current_id_list->id), symbol_table[i]->id) == 0)
               {
                  symbol_table[i]->type_number = rhs_number;
               }
              
              }
               current_id_list = current_id_list->id_list;
            }
            currentVarList = currentVarList->var_decl_list;
        }
         
    }
   
   
   //************************
   
   
     int analysis = parser.analysis_body(program->body);
    if (analysis < 0)
    {
        return 1;
    }

   
    int type_array[5 + t_counter];
    int i, j, k;
    for (i = 0; i < (5 + t_counter); i++)
    {
        type_array[i] = 0;
    }
    for (i = 0; i < (5 + t_counter); i++)
    { 
        int count = 0;        
        for (k = i; k < (5 + t_counter); k++)
        {
            if (symbol_table[k]->type_number == symbol_table[i]->type_number)
            {
                ++count;
            }
        }
       
        if(type_array[i] == 0)
        {
            cout<< symbol_table[i]->id;
            type_array[i] = 1;
                    
            for (j = i; j < (5 + t_counter); j++)
            {
                 if((type_array[j] == 0) && (symbol_table[j]->flag == 0) && (symbol_table[j]->type_number == symbol_table[i]->type_number))
                 {
                    cout<< " " << symbol_table[j]->id;
                    type_array[j] = 1;
                 }
            }

           
            for (j = i; j < (5 + t_counter); j++)
            {
                 if((type_array[j] == 0) && (symbol_table[j]->flag == 1) && (symbol_table[j]->type_number == symbol_table[i]->type_number))
                 {
                    cout<< " " << symbol_table[j]->id;
                    type_array[j] = 1;
                 }
            }
            cout<< " # " << endl;
        }
    }
    return 0;

}


