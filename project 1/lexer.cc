/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM","BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c,c1,c2,c3,c4,c5,c6,c7,c8;
    int flag_first_zero = 0;
    int flag_8 = 0;
    int flag_16 = 0;
    int flag_base = 0;
    int flag_base2 = 0;
    //int count = 0;
    char table[100];
    char table2[100];
    string temp1 = "";
    string temp2 = "";
    string temp3 = "";
    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
            flag_first_zero = 1;
           } else {
            tmp.lexeme = "";
             
             
             while ((!input.EndOfInput() && isdigit(c)/*||c== 'A' || c == 'B' || c== 'C' || c == 'D' || c== 'E' || c== 'F'*/)) {
                tmp.lexeme += c;
                input.GetChar(c);
                if( c == '8' || c == '9')
                {
                  flag_8 = 1;
                }
                if( c == '0')
                {
                  flag_base = 1;
                }
            }
            
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
            
            //
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
             
             
              input.GetChar(c1);
              
              if( c1 == 'x' /*&& flag_8 == 0 */)
               {
                 input.GetChar(c2);
                 if(c2 == ' ')
                 {
                   input.UngetChar(c2);
                 }
                 if(c2 == '0')
                 {
                   input.GetChar(c3);
                   if(c3 == '8' && flag_8 == 0)
                   {
                     tmp.lexeme = tmp.lexeme+c1+c2+c3;
                     tmp.token_type = BASE08NUM;
                     tmp.line_no = line_no;
                     return tmp;
                   }
                   else
                   {
                     input.UngetChar(c3);
                   }
                   input.UngetChar(c2);
                 }
                 
                  else if(c2 == '1')
                  {
                  input.GetChar(c3);
                  if(c3 == '6')
                  {
                   tmp.lexeme = tmp.lexeme +c1+c2+c3;
                   tmp.token_type = BASE16NUM;
                   tmp.line_no = line_no;
                   return tmp;
                  }
                  else
                  {
                     input.UngetChar(c3);                   
                  }
                  input.UngetChar(c2);
                 }
              
              
                     input.UngetChar(c1);
               }
               else if( c1 == '.')
               {
                   int count1 = 0;
                   int count2 = 0;
                   string temp2;
                   
                   input.GetChar(c2);
                   if(isdigit(c2))
                   {
                     //tmp.lexeme += ".";
                     //temp2 += ".";
                     while (!input.EndOfInput() && isdigit(c2))
                    {
                     count1++;
                     if(c2 == '0')
                     {
                       count2++;
                     }
                     //tmp.lexeme += c2;
                     temp2 += c2;
                     input.GetChar(c2);
                     
                    }
                  if (!input.EndOfInput()) 
                   {
                       input.UngetChar(c2);
                   }
                  
                  if(count1 == count2 && flag_first_zero == 1)
                  {
                  
                   input.UngetString(temp2);
                  
                  }
                  else
                  {
                   input.UngetString(temp2);
                    input.GetChar(c2);
                    tmp.lexeme += '.';
                    while (!input.EndOfInput() && (isdigit(c2)))
                    {
                      tmp.lexeme += c2;
                      input.GetChar(c2);
                    }
                     if (!input.EndOfInput()) {
                      input.UngetChar(c2);
                    }
                    
                   tmp.token_type = REALNUM;
                   tmp.line_no = line_no;
                   return tmp;
                  }
                  
                   }
                   
                   else
                   {
                     input.UngetChar(c2);
                     
                   }
                   input.UngetChar(c1);
                   
                    
               
               
               
               }
               
               else if(c1== 'A' || c1 == 'B' || c1 == 'C' || c1 == 'D' || c1 == 'E' || c1 == 'F')
              {
                int count = 0;
                int count2 = 0;
                
                input.UngetChar(c1);
                input.GetChar(table[count]);
                while(isdigit(table[count])||(!input.EndOfInput()&& table[count]== 'A' || table[count] == 'B' || table[count] == 'C' || table[count] == 'D' || table[count] == 'E' || table[count] == 'F' ))
                {
                  count++;
                  count2++;
                  input.GetChar(table[count]);
                }
                if(table[count] == 'x')
                {
                  input.GetChar(table[count+1]);
                  if(table[count+1] == '1')
                  {
                   input.GetChar(table[count+2]);
                   if(table[count+2] == '6')
                   {
                   
                     
                            
                            while(count > -3)
                            {
                              input.UngetChar(table[count + 2]);
                              count--;
                            }
                            
                            while(count2 >  -3)
                            {
                                count2--;
                                input.GetChar(c);
                                tmp.lexeme += c;
                                
                            }
                         
                        tmp.token_type = BASE16NUM;
                        tmp.line_no = line_no;
                        return tmp;  
            
                        
                   
                   }
                   else
                   {
                       input.UngetChar(table[count+2]);
                       input.UngetChar(table[count+1]);
                   }
                   
                }
                else
                {
                   input.UngetChar(table[count+1]);
                }
              }
              /*
              else
              {
                 input.UngetChar(table[count]);
              }
              */
                while(count > 0)
                {
                 input.UngetChar(table[count]);
                 count--;
                }
                input.UngetChar(c1);
              }
              
               else 
               {     
                     input.UngetChar(c1);
                     
               }
               
              
               
               
               
        
               
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
       
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
