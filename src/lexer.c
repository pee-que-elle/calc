#include "lexer.h"
#include "linkedlist.h"
#include "operator.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <gmp.h>

LexerToken_T* create_emptytoken(TokenType type)
{
    LexerToken_T *tok = malloc(sizeof(LexerToken_T));
    tok->type = type;
    tok->value = NULL;
    tok->original_tokenc = 0; /* throw error if still equals zero later */
    return tok;

}
LexerToken_T* create_filledtoken(TokenType type, void* value, size_t n)
{
    /* creates a token from a null-terminated string */
    LexerToken_T *tok = create_emptytoken(type);
    tok->value = malloc(n);
    strncpy(tok->value, value, n);
    return tok;
}

LinkedList_T* lex(char* input)
{
    LinkedList_T* toks = create_emptyll();
    LinkedList_T *current_node = toks;

    size_t tokc = 0;

    for(size_t i = 0, inputc = strlen(input); i < inputc;)
    {
        #if IGNORE_WHITESPACE
            while(isspace(input[i]) && input[i] != 0) i++;
            if(input[i] == 0) break;
        #endif // IGNORE_WHITESPACE

        LexerToken_T *current_token = tokenize(&input[i]);
        if(current_token == NULL || current_token->original_tokenc == 0)
        {
            free_ll(toks);
            return NULL;
        }
        
        i += current_token->original_tokenc;
        
        current_node->value = malloc(sizeof *current_token);
        memcpy(current_node->value, current_token, sizeof *current_token);

        current_node = append_llnode(current_node, create_emptyll());
         
        free(current_token);
    }
    current_node->next = NULL;
    current_node->value = NULL; /* these 2 should be nulled already but whatever */
    return toks;
}

LexerToken_T* tokenize(char *input)
{

    #define CHECK(x)                        \
        result = x(input);                  \
        if(result != NULL) return result;
        
    LexerToken_T *result = NULL;
    CHECK(tokenize_comma);    
    CHECK(tokenize_parens);
    CHECK(tokenize_operator);

    CHECK(tokenize_floatliteral);
    CHECK(tokenize_intliteral);
    CHECK(tokenize_stringliteral);
    CHECK(tokenize_identifier);
    printf("Did not pass check for '%s'\n", input); 
    return NULL;

    #undef CHECK
}

LexerToken_T* tokenize_stringliteral(char *input)
{
    if(input[0] != '"') return NULL;

    for(int i = 1; input[i] != 0; ++i)
    { 
        if(input[i] == '"')
        {
            /* string content range is [1, i-1], length is i - 2 */
            size_t len = i - 2;
            LexerToken_T *result = create_filledtoken(TOKEN_STRING, &result[1], len+1);
            result->value[len] = 0; /* null-terminate string properly */
            result->original_tokenc = len + 2; 
            return result;
        }         
    }
    return NULL; /* Reached end of input withput encountering a '"' */
}

LexerToken_T* tokenize_intliteral(char *input)
{
    #define CHECKBASE(c, b)                     \
        if(input[1] == c && input[0] == '0')    \
        {                                       \
            base = b;                           \
            basec = 2;                          \
            goto endbasecheck;                  \
        }

    int basec = 0;
    int count = 0;
    int base = 10;
    
    CHECKBASE('x', 16);
    CHECKBASE('b', 2);
    CHECKBASE('o', 8);
    
    endbasecheck:
    while(1) 
    {
        char cur = input[basec+count];

        if(cur == 0) break; /* not technically needed but might save some headaches */
        int status = 0;

        if(base <= 10) count += (status = (cur >= '0' && cur <= ('0' + base - 1) ));
        else if (base == 16) count += (status = !!isxdigit(cur));
        else; /**/

        if(status == 0) break;
    }

    if(count == 0) return NULL;

    LexerToken_T *result = create_filledtoken(TOKEN_INT, input, basec+count+1);

    result->value[basec+count] = 0;

    result->original_tokenc = basec + count;

    return result;

    #undef CHECKBASE
}

LexerToken_T* tokenize_floatliteral(char *input)
{
    int i, encountered_dot;
    for(encountered_dot = 0, i = 0; input[i] != 0; i++)
    {
        char cur = input[i];
        
        if(cur == '.')
        {
            if(encountered_dot) return NULL;
            encountered_dot = 1;
        }
        else if(!isdigit(cur)) break;
    }

    if(!encountered_dot || input[i-1] == '.') return NULL;

    
    LexerToken_T *result = create_filledtoken(TOKEN_FLOAT, input, i+1);
    result->value[i] = 0;

    result->original_tokenc = i;

    return result;
}

LexerToken_T* tokenize_operator(char *input)
{
    char *touse = calloc(0,1);
    for(int i = 0; i  < (sizeof operators/sizeof *operators); ++i)
    {
        
        char *current = operators[i].identifier;
        size_t current_len = strlen(current);
        if(strncmp(current, input, current_len) == 0 && current_len > strlen(touse))
        {
            /* Fill value so we don't have to do a lookup when pretty-printing later */
            touse = realloc(touse, current_len + 1);
            strcpy(touse, current);
        }
    }
    if(touse[0] == 0) return NULL;
    
    size_t touse_len = strlen(touse);
    LexerToken_T* result = create_filledtoken(TOKEN_OPERATOR, touse, touse_len+1);
    result->value[touse_len] = 0;
    result->original_tokenc = touse_len;
    return result;
}

LexerToken_T *tokenize_identifier(char *input)
{

    int i;

    for(i = 0; input[i] != 0; ++i)
    {
        
        if(!(input[i] == '_' || isalpha(input[i]) || (i != 0 && isdigit(input[i]) )))
        {
            break;
        }
    }
    if(i == 0) return NULL;
    LexerToken_T *result = create_filledtoken(TOKEN_IDENTIFIER, input, i+1);
    result->value[i] = 0;
    result->original_tokenc = i;
    return result;
    
}

LexerToken_T* tokenize_parens(char* input)
{
    LexerToken_T *result;
    switch(*input)
    {
        case '(' :
        case ')' :
            result = create_emptytoken(*input == '(' ? TOKEN_LPAREN : TOKEN_RPAREN);
            result->original_tokenc = 1;
            break;
        default:
            result = NULL;
    }
    return result;
}
LexerToken_T* tokenize_comma(char *input)
{
    if(*input != ',') return NULL;

    LexerToken_T *result = create_emptytoken(TOKEN_COMMA);
    result->original_tokenc = 1;
    return result;
}

char *toktype2str(TokenType t)
{
   switch(t)
   {
        case TOKEN_STRING:
            return "string";
        case TOKEN_INT:
            return "int";
        case TOKEN_FLOAT:
            return "float";
        case TOKEN_OPERATOR:
            return "operator";
        case TOKEN_LPAREN:
            return "lparen";
        case TOKEN_RPAREN:
            return "rparen";
        case TOKEN_COMMA:
            return "comma";
        case TOKEN_IDENTIFIER:
            return "identifier";
        default: return "????";
   } 
}

char *tok2str(LexerToken_T *tok)
{
    #define CHAR2STR(c)         \
        result = calloc(0, 2);  \
        result[0] = c;
    char *result;

    switch(tok->type)
    {
        case TOKEN_STRING:
        case TOKEN_INT:
        case TOKEN_FLOAT:
        case TOKEN_OPERATOR:
        case TOKEN_IDENTIFIER:
            result = malloc(strlen(tok->value)+1);
            strcpy(result, tok->value);
            break;
        case TOKEN_LPAREN:
            CHAR2STR('(');
            break;
        case TOKEN_RPAREN:
            CHAR2STR(')');
            break;
        case TOKEN_COMMA:
            CHAR2STR(',');
            break;
    }
    return result;
}
