#include "compiler.cc"
#include <string.h>
#include <stdlib.h>

#define MAX_VARS 20
#define ALLOC(t) (t*) calloc(1,sizeof(t))

struct ValueNode* table[MAX_VARS];
int number_of_vars = 0;
int number_of_ints = 0;

struct StatementNode* parse_body();



struct ValueNode* get_symbol(char* name)
{
 int i;
 for(i = 0; i < number_of_vars; i++)
{
   if(strcmp(name, table[i]->name) == 0)
   {
   return table[i];
   
   }
    return NULL;
}

}

void parse_id_list()
{
  while (ttype!=SEMICOLON)
  {
   ttype = getToken();
   table[MAX_VARS] = ALLOC(struct ValueNode);
   table[MAX_VARS] -> value = 0;
   table[MAX_VARS] -> name = malloc(strlen(token)+1);
   strcpy(table[MAX_VARS]->name,token);
   number_of_vars++;
   ttype = getToken();
   }
 }
 
 struct StatementNode* parse_assign()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    st->next = NULL;
    ttype = getToken();
    if (ttype == ID)
    {
        st->type = ASSIGN_STMT;
        st->assign_stmt = ALLOC(struct AssignmentStatement);
        st->assign_stmt->left_hand_side = get_symbol(token);
    }
    ttype = getToken(); // EQUAL
    if (ttype == EQUAL)
    {
        ttype = getToken();
        switch (ttype)
            {
                case ID:
                    st->assign_stmt->operand1 = get_symbol(token);
                    break;
                case NUM:
                    table[number_of_vars+number_of_ints] = ALLOC(struct ValueNode);
                    table[number_of_vars+number_of_ints]->value = atoi(token);
                    table[number_of_vars+number_of_ints]->name = "";    
                    st->assign_stmt->operand1 = table[number_of_vars+number_of_ints];
                    ++number_of_ints;
                    break;
            }
        ttype = getToken();
        if (ttype == SEMICOLON)
        {
            st->assign_stmt->op = 0;
            st->assign_stmt->operand2 = NULL;
        }
        else
        {
            st->assign_stmt->op = ttype;
            ttype = getToken();
            switch (ttype)
            {
                case ID:
                    st->assign_stmt->operand2 = get_symbol(token);
                    break;
                case NUM:
                    table[number_of_vars+number_of_ints] = ALLOC(struct ValueNode);
                    table[number_of_vars+number_of_ints]->value = atoi(token);
                    table[number_of_vars+number_of_ints]->name = "";    
                    st->assign_stmt->operand2 = table[number_of_vars+number_of_ints];
                    ++number_of_ints;
                    break;
            }
            ttype = getToken(); // SEMICOLON
        }
    }
    return st;
}

struct StatementNode* parse_print()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    st->next = NULL;
    ttype = getToken();
    if (ttype == PRINT)
    {
        ttype = getToken();
        st->type = PRINT_STMT;
        st->print_stmt = ALLOC(struct PrintStatement);
        st->print_stmt->id = get_symbol(token);
    }
    ttype = getToken(); // SEMICOLON
    return st;
}

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
 
    ttype = getToken();
    if (ttype == WHILE)
    {
        // Parse condition
        ttype = getToken();  //operand1
        switch (ttype)
        {
            case ID:
                st->if_stmt->condition_operand1 = get_symbol(token);
                break;
            case NUM:
                table[number_of_vars+number_of_ints] = ALLOC(struct ValueNode);
                   table[number_of_vars+number_of_ints]->value = atoi(token);
                   table[number_of_vars+number_of_ints]->name = "";    
                   st->if_stmt->condition_operand1 = table[number_of_vars+number_of_ints];
                   ++number_of_ints;
                   break;
        }
        ttype = getToken(); // Op
        st->if_stmt->condition_op = ttype;
        ttype = getToken(); //operand2
        switch (ttype)
        {
                case ID:
                   st->if_stmt->condition_operand2 = get_symbol(token);
                   break;
                case NUM:
                   table[number_of_vars+number_of_ints] = ALLOC(struct ValueNode);
                   table[number_of_vars+number_of_ints]->value = atoi(token);
                   table[number_of_vars+number_of_ints]->name = "";    
                   st->if_stmt->condition_operand2 = table[number_of_vars+number_of_ints];
                   ++number_of_ints;
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
struct StatementNode* parse_if()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    struct StatementNode* end = ALLOC(struct StatementNode);
    end->type = NOOP_STMT;
    st->type = IF_STMT;
    st->if_stmt = ALLOC(struct IfStatement);
    st->next = end;
    end->next = NULL;

    ttype = getToken();
    if (ttype == IF)
    {
        // Parse condition
        ttype = getToken(); //operand1
        switch (ttype)
        {
                case ID:
                   st->if_stmt->condition_operand1 = get_symbol(token);
                   break;
                case NUM:
                   table[number_of_vars+number_of_ints] = ALLOC(struct ValueNode);
                   table[number_of_vars+number_of_ints]->value = atoi(token);
                   table[number_of_vars+number_of_ints]->name = "";    
                   st->if_stmt->condition_operand1 = table[number_of_vars+number_of_ints];
                   ++number_of_ints;
                   break;
        }
        ttype = getToken(); // Op
        st->if_stmt->condition_op = ttype;
        ttype = getToken(); //operand2
        switch (ttype)
        {
                case ID:
                   st->if_stmt->condition_operand2 = get_symbol(token);
                   break;
                case NUM:
                   table[number_of_vars+number_of_ints] = ALLOC(struct ValueNode);
                   table[number_of_vars+number_of_ints]->value = atoi(token);
                   table[number_of_vars+number_of_ints]->name = "";    
                   st->if_stmt->condition_operand2 = table[number_of_vars+number_of_ints];
                   ++number_of_ints;
                   break;
        }
        st->if_stmt->true_branch = parse_body();
        st->if_stmt->false_branch = end;
        struct StatementNode* current = st->if_stmt->true_branch;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = end;
    }    
    return st;
}

struct StatementNode* parse_case()
{
    struct StatementNode* st = ALLOC(struct StatementNode);
    st->if_stmt = ALLOC(struct IfStatement);
    st->type = IF_STMT;
    st->next = NULL;
    st->if_stmt->condition_op = NOTEQUAL;
    st->if_stmt->true_branch = NULL;
    ttype = getToken();
    if (ttype == CASE)
    {
        ttype = getToken(); //operand2
        table[number_of_vars+number_of_ints] = ALLOC(struct ValueNode);
        table[number_of_vars+number_of_ints]->value = atoi(token);
        table[number_of_vars+number_of_ints]->name = "";    
        st->if_stmt->condition_operand2 = table[number_of_vars+number_of_ints];
        ++number_of_ints;
        ttype = getToken(); //COLON
        st->if_stmt->false_branch = parse_body();
    }
    return st;
}

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

    ttype = getToken();
    if (ttype == SWITCH)
    {
        // Parse condition
        ttype = getToken();  //operand1
        var = get_symbol(token);
        ttype = getToken(); // LBRACE
        ttype = getToken(); // CASE
        while (ttype == CASE)
        {
            ungetToken();            
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
            ttype = getToken();
        }
        if (ttype == DEFAULT)
        {
            ttype = getToken(); // COLON
            st->next = parse_body();
            st->if_stmt->true_branch = st->next;
        }
        struct StatementNode *current = st;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = end;
    }
    return st;
}

struct StatementNode* parse_stmt()
{
	struct StatementNode* st;
	ttype = getToken();
	switch (ttype)
	{
		case ID:
			ungetToken();
			st = parse_assign();
			break;
		case PRINT:
			ungetToken();
			st = parse_print();
			break;
		case WHILE:
			ungetToken();
			st = parse_while();
			break;
		case IF:
			ungetToken();
			st = parse_if();
			break;
		case SWITCH:
			ungetToken();
			st = parse_switch();
			break;	
	}
	return st;
}

struct StatementNode* parse_stmt_list()
{
    struct StatementNode *st, *stl;
    st = parse_stmt();
    ttype = getToken();
    if ((ttype == ID) || (ttype == PRINT) || (ttype == WHILE) || (ttype == IF) || (ttype == SWITCH))
    {
        ungetToken();
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
        ungetToken();
    }
    return st;
}

struct StatementNode* parse_body()
{
	struct StatementNode* stl;
	ttype = getToken();
	if (ttype == LBRACE)
	{
		stl = parse_stmt_list();
	}
	ttype = getToken(); // get the RBRACE
	return stl;
}

struct StatementNode* parse_generate_intermediate_representation()
{
  parse_id_list();
  return parse_body();
}
