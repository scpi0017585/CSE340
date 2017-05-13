/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cctype>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include "compiler.h"
#include "ir_debug.h"
#include "lexer.h"

#define DEBUG 0    // 1 => Turn ON debugging, 0 => Turn OFF debugging
#define MAX_VARS 200
#define ALLOC(t) (t*) calloc(1,sizeof(t))

struct ValueNode* table[MAX_VARS];

int int_count = 0;
int count = 0;
char* chr;
struct StatementNode* parse_body();

LexicalAnalyzer lexer;

void debug(const char* format, ...)
{
    va_list args;
    if (DEBUG)
    {
        va_start (args, format);
        vfprintf (stdout, format, args);
        va_end (args);
    }
}

void execute_program(struct StatementNode * program)
{
    struct StatementNode * pc = program;
    int op1, op2, result;

    while (pc != NULL)
    {
        switch (pc->type)
        {
            case NOOP_STMT:
                pc = pc->next;
                break;

            case PRINT_STMT:
                if (pc->print_stmt == NULL)
                {
                    debug("Error: pc points to a print statement but pc->print_stmt is null.\n");
                    exit(1);
                }
                if (pc->print_stmt->id == NULL)
                {
                    debug("Error: print_stmt->id is null.\n");
                    exit(1);
                }
                printf("%d\n", pc->print_stmt->id->value);
                pc = pc->next;
                break;

            case ASSIGN_STMT:
                if (pc->assign_stmt == NULL)
                {
                    debug("Error: pc points to an assignment statement but pc->assign_stmt is null.\n");
                    exit(1);
                }
                if (pc->assign_stmt->operand1 == NULL)
                {
                    debug("Error: assign_stmt->operand1 is null.\n");
                    exit(1);
                }
                if (pc->assign_stmt->op != OPERATOR_NONE)
                {
                    if (pc->assign_stmt->operand2 == NULL)
                    {
                        debug("Error: assign_stmt->op requires two operands but assign_stmt->operand2 is null.\n");
                        exit(1);
                    }
                }
                if (pc->assign_stmt->left_hand_side == NULL)
                {
                    debug("Error: assign_stmt->left_hand_side is null.\n");
                    exit(1);
                }
                switch (pc->assign_stmt->op)
                {
                    case OPERATOR_PLUS:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 + op2;
                        break;
                    case OPERATOR_MINUS:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 - op2;
                        break;
                    case OPERATOR_MULT:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 * op2;
                        break;
                    case OPERATOR_DIV:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 / op2;
                        break;
                    case OPERATOR_NONE:
                        op1 = pc->assign_stmt->operand1->value;
                        result = op1;
                        break;
                    default:
                        debug("Error: invalid value for assign_stmt->op (%d).\n", pc->assign_stmt->op);
                        exit(1);
                        break;
                }
                pc->assign_stmt->left_hand_side->value = result;
                pc = pc->next;
                break;

            case IF_STMT:
                if (pc->if_stmt == NULL)
                {
                    debug("Error: pc points to an if statement but pc->if_stmt is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->true_branch == NULL)
                {
                    debug("Error: if_stmt->true_branch is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->false_branch == NULL)
                {
                    debug("Error: if_stmt->false_branch is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->condition_operand1 == NULL)
                {
                    debug("Error: if_stmt->condition_operand1 is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->condition_operand2 == NULL)
                {
                    debug("Error: if_stmt->condition_operand2 is null.\n");
                    exit(1);
                }
                op1 = pc->if_stmt->condition_operand1->value;
                op2 = pc->if_stmt->condition_operand2->value;
                switch (pc->if_stmt->condition_op)
                {
                    case CONDITION_GREATER:
                        if (op1 > op2)
                            pc = pc->if_stmt->true_branch;
                        else
                            pc = pc->if_stmt->false_branch;
                        break;
                    case CONDITION_LESS:
                        if (op1 < op2)
                            pc = pc->if_stmt->true_branch;
                        else
                            pc = pc->if_stmt->false_branch;
                        break;
                    case CONDITION_NOTEQUAL:
                        if (op1 != op2)
                            pc = pc->if_stmt->true_branch;
                        else
                            pc = pc->if_stmt->false_branch;
                        break;
                    default:
                        debug("Error: invalid value for if_stmt->condition_op (%d).\n", pc->if_stmt->condition_op);
                        exit(1);
                        break;
                }
                break;

            case GOTO_STMT:
                if (pc->goto_stmt == NULL)
                {
                    debug("Error: pc points to a goto statement but pc->goto_stmt is null.\n");
                    exit(1);
                }
                if (pc->goto_stmt->target == NULL)
                {
                    debug("Error: goto_stmt->target is null.\n");
                    exit(1);
                }
                pc = pc->goto_stmt->target;
                break;

            default:
                debug("Error: invalid value for pc->type (%d).\n", pc->type);
                exit(1);
                break;
        }
    }
}

int main()
{
    
    struct StatementNode * program;
    program = parse_generate_intermediate_representation();
    if (DEBUG)
    {
        printf("\n-------------------------------------------\n");
        print_program(program);
        printf("\n-------------------------------------------\n");
    }
    execute_program(program);
    return 0;
}




struct ValueNode* match_up(char* name)
{
 
 for(int i = 0; i < count; i++)
{
   if(strcmp(name, table[i]->name.c_str()) == 0)
   {
   return table[i];
   
   }
   else
   {
    return NULL;
   }
}

}

void parse_id_list()  //parse id list, first line
{


  Token t = lexer.GetToken();
  
  
  while (t.token_type != SEMICOLON)    // read the ; then jump to next line, { 
  {
   
   //table[MAX_VARS] = ALLOC(struct ValueNode);
   table[count] = new ValueNode{};
   table[count] -> value = 0;
   const char *temp = (t.lexeme).c_str();
   //table[MAX_VARS] -> name = malloc(strlen((t.lexeme).c_str())+1);
    //table[MAX_VARS] -> name = new char[(t.lexeme).length() + 1];
    //table[MAX_VARS] -> name = new ValueNode{};
   /*chr = strdup((t.lexeme).c_str());
   strcpy(table[MAX_VARS]->name,chr); */
   table[count] -> name = t.lexeme;
   //count++;
   count++;
   t = lexer.GetToken();
   }
 }
 
 struct StatementNode* parse_assign()
{    struct StatementNode* st = ALLOC(struct StatementNode);
   
    st->next = NULL;
    Token t = lexer.GetToken();
    //char* chr = strdup((t.lexeme).c_str());
    if (t.token_type == ID)
    {
    
    
        st->type = ASSIGN_STMT;
        
        
        st->assign_stmt = ALLOC(struct AssignmentStatement);
        //char* chr = strdup((t.lexeme).c_str());
        //chr = strdup((t.lexeme).c_str());
        //st->assign_stmt->left_hand_side = match_up(chr);
        
        for(int i =0; i< count; i++)
        {
           if(table[i]->name == t.lexeme)
           {
           st->assign_stmt->left_hand_side = table[i];
           }

        }
    }
    t = lexer.GetToken(); // t.token_type = EQUAL
    
    if (t.token_type == EQUAL)
    {
        t = lexer.GetToken();
        switch (t.token_type)
            {
                case ID:
                    //char* chr2 = strdup((t.lexeme).c_str());
                    //chr = strdup((t.lexeme).c_str());
                    //st->assign_stmt->operand1 = match_up(chr);
                     for(int i =0; i< count; i++)
                     {
                     if(table[i]->name == t.lexeme)
                     {
                       st->assign_stmt->operand1 = table[i];
                     }

                     }
                    break;
                case NUM:
                    table[count+int_count] = new ValueNode{};
                    chr = strdup((t.lexeme).c_str());
                    table[count+int_count]->value = atoi(chr);
                    table[count+int_count]->name = "";    
                    st->assign_stmt->operand1 = table[count+int_count];
                    int_count++;
                    break;
            }
        t = lexer.GetToken();
        if (t.token_type == SEMICOLON)
        {
            st->assign_stmt->op = OPERATOR_NONE;
            st->assign_stmt->operand2 = NULL;
            
            
        }
        else
        {
            if (t.token_type ==  PLUS)
            {
            st->assign_stmt->op = OPERATOR_PLUS;
            }
            else if(t.token_type == MINUS)
            {
             st->assign_stmt->op =  OPERATOR_MINUS;
            }
            else if(t.token_type == MULT)
            {
              st->assign_stmt->op =  OPERATOR_MULT;
            }
            else if(t.token_type == DIV)
            {
               st->assign_stmt->op =  OPERATOR_DIV;
            }
            t = lexer.GetToken();
            switch (t.token_type)
            {
                case ID:
                    //char* chr3 = strdup((t.lexeme).c_str());
                    //chr = strdup((t.lexeme).c_str());
                    //st->assign_stmt->operand2 = match_up(chr);
                     for(int i =0; i< count; i++)
                     {
                      if(table[i]->name == t.lexeme)
                     {
                      st->assign_stmt->operand2 = table[i];
                     }

                     }
                    break;
                case NUM:
                    table[count+int_count] = new ValueNode{};
                    chr = strdup((t.lexeme).c_str());
                    table[count+int_count]->value = atoi(chr);
                    table[count+int_count]->name = "";    
                    st->assign_stmt->operand2 = table[count+int_count];
                    int_count++;
                    break;
            }
            t = lexer.GetToken(); // t.token_type = SEMICOLON
        }
    }
    return st;
}

/*
struct StatementNode* parse_case()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    st->if_stmt = ALLOC(struct IfStatement);
    st->type = IF_STMT;
    st->next = NULL;
    st->if_stmt->condition_op = CONDITION_NOTEQUAL;
    st->if_stmt->true_branch = NULL;
    Token t = lexer.GetToken();
    if (t.token_type == CASE)
    {
        t = lexer.GetToken(); //operand2
        table[count+int_count] = new ValueNode{};
        chr = strdup((t.lexeme).c_str());
        table[count+int_count]->value = atoi(chr);
        table[count+int_count]->name = "";    
        st->if_stmt->condition_operand2 = table[count+int_count];
        ++int_count;
        t = lexer.GetToken(); //COLON
        st->if_stmt->false_branch = parse_body(); //body
    }
    return st;
}

*/
struct StatementNode* parse_while()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    struct StatementNode* end = ALLOC(struct StatementNode);
    struct StatementNode* loop = ALLOC(struct StatementNode);
    loop->type = GOTO_STMT;
    end->type = NOOP_STMT;
    st->type = IF_STMT;
    st->if_stmt = ALLOC(struct IfStatement);
    st->next = end;
    loop->next = NULL;
    end->next = NULL;
    loop->goto_stmt = ALLOC(struct GotoStatement);
 
    Token t = lexer.GetToken();
    if (t.token_type == WHILE)
    {
        // Parse condition
        t = lexer.GetToken();  //operand1
        switch (t.token_type)
        {
            case ID:
                //chr = strdup((t.lexeme).c_str());
                //st->if_stmt->condition_operand1 = match_up(chr);
                 for(int i =0; i< count; i++)
                 {
                      if(table[i]->name == t.lexeme)
                 {
                      st->if_stmt->condition_operand1 = table[i];
                 }

                 }
                break;
            case NUM:
                table[count+int_count] = new ValueNode{};
                chr = strdup((t.lexeme).c_str());
                   table[count+int_count]->value = atoi(chr);
                   table[count+int_count]->name = "";    
                   st->if_stmt->condition_operand1 = table[count+int_count];
                   ++int_count;
                   break;
        }
        t = lexer.GetToken(); // Op
        //st->if_stmt->condition_op = t.token_type;
          if(t.token_type == LESS )
        {
        st->if_stmt->condition_op = CONDITION_LESS;
        }
        else if(t.token_type == GREATER)
        {
        st->if_stmt->condition_op = CONDITION_GREATER;
        }
        else if(t.token_type == NOTEQUAL)
        {
        st->if_stmt->condition_op =  CONDITION_NOTEQUAL;
        }
        t = lexer.GetToken(); //operand2
        switch (t.token_type)
        {
                case ID:
                   //chr = strdup((t.lexeme).c_str());
                   //st->if_stmt->condition_operand2 = match_up(chr);
                    for(int i =0; i< count; i++)
        {
           if(table[i]->name == t.lexeme)
           {
           st->if_stmt->condition_operand2 = table[i];
           }

        }
                   break;
                case NUM:
                   table[count+int_count] = new ValueNode{};
                   chr = strdup((t.lexeme).c_str());
                   table[count+int_count]->value = atoi(chr);
                   table[count+int_count]->name = "";    
                   st->if_stmt->condition_operand2 = table[count+int_count];
                   ++int_count;
                   break;
        }
        st->if_stmt->true_branch = parse_body();
        st->if_stmt->false_branch = end;
        loop->goto_stmt->target = st;
        struct StatementNode* current = st->if_stmt->true_branch;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = loop;
    }
    return st;
}

struct StatementNode* parse_print()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    //st->next = NULL;
    Token t = lexer.GetToken();
    if (t.token_type == PRINT)
    {
        t = lexer.GetToken();
        st->type = PRINT_STMT;
        st->print_stmt = ALLOC(struct PrintStatement);
        //chr = strdup((t.lexeme).c_str());
        //st->print_stmt->id = match_up(chr);
         for(int i =0; i< count; i++)
        {
           if(table[i]->name == t.lexeme)
           {
           st->print_stmt->id = table[i];
           }

        }
    }
    t = lexer.GetToken(); // SEMICOLON
    return st;
}

struct StatementNode* parse_if()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    
    struct StatementNode* st_end = ALLOC(struct StatementNode);
    st_end->type = NOOP_STMT;
    st->type = IF_STMT;
    st->if_stmt = ALLOC(struct IfStatement);
    st->next = st_end;
    st_end->next = NULL;

    Token t = lexer.GetToken();
    if (t.token_type == IF)
    {
        // Parse condition
        t = lexer.GetToken(); //operand1
        switch (t.token_type)
        {
                case ID:
                   //chr = strdup((t.lexeme).c_str());
                   //st->if_stmt->condition_operand1 = match_up(chr);
                    for(int i =0; i< count; i++)
        {
           if(table[i]->name == t.lexeme)
           {
           st->if_stmt->condition_operand1 = table[i];
           }

        }
                   break;
                case NUM:
                   table[count+int_count] = new ValueNode{};
                   chr = strdup((t.lexeme).c_str());
                   table[count+int_count]->value = atoi(chr);
                   table[count+int_count]->name = "";    
                   st->if_stmt->condition_operand1 = table[count+int_count];
                   ++int_count;
                   break;
        }
        t = lexer.GetToken(); // Op
        if(t.token_type == LESS )
        {
        st->if_stmt->condition_op = CONDITION_LESS;
        }
        else if(t.token_type == GREATER)
        {
        st->if_stmt->condition_op = CONDITION_GREATER;
        }
        else if(t.token_type == NOTEQUAL)
        {
        st->if_stmt->condition_op =  CONDITION_NOTEQUAL;
        }
        t = lexer.GetToken(); //operand2
        switch (t.token_type)
        {
                case ID:
                   //chr = strdup((t.lexeme).c_str());
                   //st->if_stmt->condition_operand2 = match_up(chr);
                    for(int i =0; i< count; i++)
        {
           if(table[i]->name == t.lexeme)
           {
           st->if_stmt->condition_operand2 = table[i];
           }

        }
                   break;
                case NUM:
                   table[count+int_count] = new ValueNode{};
                   chr = strdup((t.lexeme).c_str());
                   table[count+int_count]->value = atoi(chr);
                   table[count+int_count]->name = "";    
                   st->if_stmt->condition_operand2 = table[count+int_count];
                   ++int_count;
                   break;
        }
        st->if_stmt->true_branch = parse_body();
        st->if_stmt->false_branch = st_end;
        struct StatementNode* current = st->if_stmt->true_branch;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = st_end;
    }    
    return st;
}

/*
struct StatementNode* parse_switch()
{
    struct StatementNode* end = ALLOC(struct StatementNode);
    struct StatementNode* loop = ALLOC(struct StatementNode);
    
    struct StatementNode *st, *current, *old_st = NULL;
    struct ValueNode* var;
    
    loop->type = GOTO_STMT;
    end->type = NOOP_STMT;
    loop->next = NULL;
    end->next = NULL;
    loop->goto_stmt = ALLOC(struct GotoStatement);
    
    loop->goto_stmt->target = end;

    Token t = lexer.GetToken();
    if (t.token_type == SWITCH)
    {
        t = lexer.GetToken();  //get the operand1( ID )
       
       // char* chr7 = strdup((t.lexeme).c_str());
       // var = match_up(chr7);
       for(int i = 0; i < count ; i++)
       {
         if(table[i]->name == t.lexeme )
         {
         
          var = table[i];
         }
       
       
       }
        t = lexer.GetToken(); // get the LBRACE
        t = lexer.GetToken(); // get the CASE
        while (t.token_type == CASE)
        {
            lexer.UngetToken(t);          
            st = parse_case();            
            st->if_stmt->condition_operand1 = var;
            current = st->if_stmt->false_branch;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = loop;
            st->next = end;
            st->if_stmt->true_branch = end;
            if (old_st) {old_st->if_stmt->true_branch = st; old_st->next = st;}
            old_st = st;
            t = lexer.GetToken(); //DEFAULT or RBRACE
        }
        if (t.token_type == DEFAULT)
        {
            t = lexer.GetToken(); // COLON
            st->next = parse_body();
            st->if_stmt->true_branch = st->next;
        }
        struct StatementNode *current = st;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = end;
        
        
        if(t.token_type != RBRACE)
        {
           return NULL;
        }
    }
    return st;
}
*/

struct StatementNode* parse_case(char* variable,struct StatementNode* statements)
{ 
  Token t = lexer.GetToken();    // get the CASE or  DEFAULT
  struct StatementNode* st_case;
  if(t.token_type == DEFAULT || t.token_type == CASE)
  {
    st_case = ALLOC(struct StatementNode);
    st_case->type = IF_STMT;
    struct IfStatement* st_if;
    st_if = ALLOC(struct IfStatement);
    /*
    for(int i = 0; i< count; i++)
    {
      if(variable == table[i]->name )
      {
        st_if->condition_operand1 = table[i];
      }
    }
    */
    st_if->condition_operand1 = match_up(variable);
    st_if->condition_op = CONDITION_NOTEQUAL;
    if(t.token_type == CASE)
    {
      t = lexer.GetToken();   // get the NUM or ID
      if(t.token_type == NUM || t.token_type == ID)
      {
       switch(t.token_type)
       { 
        case NUM:
          table[count+int_count] = new ValueNode{};
          table[count+int_count]->name = t.lexeme;
          table[count+int_count]->value = atoi((t.lexeme).c_str());
          st_if->condition_operand2 = table[count+int_count];
          int_count++;
          break;
        case ID:
          for(int i = 0; i< count ; i++)
          {
            if(t.lexeme == table[i]->name)
            {
               st_if->condition_operand2 = table[i];
            }
          }
          break;
       }
    }
    else 
    {
      lexer.UngetToken(t);
    }
   
  
  
  
  }
else if(t.token_type == DEFAULT)
{
   for(int i = 0; i< count; i++)
   {
     if(variable == table[i]->name)
     {
        st_if->condition_operand2 = table[i];
     }

   }
}
st_case->if_stmt = st_if;
t = lexer.GetToken();   //get the COLON
if(t.token_type == COLON)
{
  st_if->false_branch = parse_body();
  struct StatementNode* statement = ALLOC(struct StatementNode);
  statement = st_if -> false_branch;
  while(statement->next != NULL)
  {
    statement = statement->next;
  }
  statement->next = statements;
  t = lexer.GetToken(); // next Token is CASE or DEFAULT
  switch(t.token_type)
  {
     case CASE:
       lexer.UngetToken(t);
       st_if->true_branch = parse_case(variable, statements);
       return st_case;
       break;
     case DEFAULT:
       lexer.UngetToken(t);
       st_if->true_branch = parse_case(variable, statements);
       return st_case;
       break;
     default:
       lexer.UngetToken(t);
       st_if->true_branch = statements;
       return st_case;
  }
}
else
{
  lexer.UngetToken(t);
}





}
else 
{
 lexer.UngetToken(t);
}
 return NULL;
}

struct StatementNode* parse_switch()
{
 struct StatementNode* st = ALLOC(struct StatementNode);
 Token t = lexer.GetToken();
 if(t.token_type == SWITCH)   // get the SWITCH
 {
  t = lexer.GetToken();       // get the ID
  st->type = NOOP_STMT;
  if(t.token_type == ID)
  {
    char* var = new char[(t.lexeme).length() + 1];  
    strcpy(var, (t.lexeme).c_str());
    t = lexer.GetToken();        // get the LBRACE
    if(t.token_type == LBRACE)
    {
      struct StatementNode *noop_st;      
      noop_st = ALLOC(struct StatementNode);
      noop_st -> type = NOOP_STMT;
      struct GotoStatement *gts;
      gts = ALLOC(struct GotoStatement);
      struct StatementNode *stm;
      stm = ALLOC(struct StatementNode);
      stm-> type = GOTO_STMT;
      stm-> goto_stmt =gts;
      st->next = parse_case(var,stm);
      st->next->next = noop_st;
      t = lexer.GetToken();       //get RBRACE
      if(t.token_type != RBRACE)
      {
           //return NULL;
      }
    }
  }
 
 }
  return st;
}


struct StatementNode* parse_stmt()
{
	struct StatementNode* st;
	Token t = lexer.GetToken();
	switch (t.token_type)
	{
		case ID:
			lexer.UngetToken(t);
			st = parse_assign();
			break;
		case PRINT:
			lexer.UngetToken(t);
			st = parse_print();
			break;
		case WHILE:
			lexer.UngetToken(t);
			st = parse_while();
			break;
		case IF:
			lexer.UngetToken(t);
			st = parse_if();
			break;
		case SWITCH:
			lexer.UngetToken(t);
			st = parse_switch();
			break;	
	}
	return st;
}

struct StatementNode* parse_stmt_list()
{
    struct StatementNode *st, *stl;
    st = parse_stmt();
    Token t = lexer.GetToken();
    if ((t.token_type == ID) || (t.token_type == WHILE) || (t.token_type == PRINT) || (t.token_type == IF) || (t.token_type == SWITCH))
    {
        lexer.UngetToken(t);
        stl = parse_stmt_list();
	    if (st->type == IF_STMT)
	    {
            struct StatementNode *current = st->next;	  
            while (current->type != NOOP_STMT)
            {
                current = current->next;
            }
            current->next = stl;
	    }
	    else
	    {
	      st->next = stl;
	    } 
    }
    else
    {
        lexer.UngetToken(t);
    }
    return st;
}

struct StatementNode* parse_body()
{
	struct StatementNode* stl;
	Token t = lexer.GetToken(); // get the { 
	if (t.token_type == LBRACE)
	{
		stl = parse_stmt_list();  // parse the statement list
	}
	t = lexer.GetToken(); // get the RBRACE
	return stl;
}

struct StatementNode* parse_generate_intermediate_representation()
{
  parse_id_list();  // first line of declaration 
  return parse_body();  // parse the body
}
