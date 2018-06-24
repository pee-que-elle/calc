#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <gmp.h>

LexerToken* create_emptytoken(TokenType type)
{
    LexerToken *tok = malloc(sizeof(LexerToken));
    tok->type = type;
    tok->value = NULL;
    tok->original_tokenc = 0; /* throw error if still equals zero later */
    return tok;

}
LexerToken* create_filledtoken(TokenType type, void* value, size_t n)
{
    /* creates a token from a null-terminated string */
    LexerToken *tok = create_emptytoken(type);
    tok->value = malloc(n);
    memcpy(tok->value, value, n);
    return tok;
}

LexerToken* lex(char* input)
{
    #define REALLOC_TOKS(c) c = realloc(c, sizeof(LexerToken) * ++tokc)
    #define APPEND_TOKS(ts, t) memcpy(&ts[tokc-1], t, sizeof(LexerToken))
    LexerToken* toks = malloc(0);
    size_t tokc = 0;

    for(size_t i = 0, inputc = strlen(input); i < inputc;)
    {
        LexerToken *current = tokenize(&input[i]);
        if(current == NULL || current->original_tokenc == 0)
        {
            free(toks);
            return NULL;
        }
        REALLOC_TOKS(toks);
        APPEND_TOKS(toks, current);
        
        i += current->original_tokenc;
        free(current);
    }
    REALLOC_TOKS(toks);
    LexerToken *terminator = create_emptytoken(METATOKEN_TERMINATE);
    
    APPEND_TOKS(toks, terminator);

    free(terminator);
    return toks;
}

LexerToken* tokenize(char *input)
{

    #define CHECK(x)                        \
        result = x(input);                  \
        if(result != NULL) return result;
        
    LexerToken *result = NULL;
    
    CHECK(tokenize_stringliteral);
    CHECK(tokenize_intliteral);
    // CHECK(tokenize_floatliteral);
    CHECK(tokenize_operator);
    
    printf("Did not pass check for '%s'\n", input); 
    return NULL;
}

LexerToken* tokenize_stringliteral(char *input)
{
    if(input[0] != '"') return NULL;

    for(int i = 1; input[i] != 0; ++i)
    { 
        if(input[i] == '"')
        {
            /* string content range is [1, i-1], length is i - 2 */
            size_t len = i - 2;
            LexerToken *result = create_filledtoken(TOKEN_STRING, &result[1], len+1);
            ((char*)(result->value))[len] = 0; /* null-terminate string properly */
            result->original_tokenc = len + 2; 
            return result;
        }         
    }
    return NULL; /* Reached end of input withput encountering a '"' */
}

LexerToken* tokenize_intliteral(char *input)
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
        int status = 0;

        if(base <= 10) count += (status = (cur >= '0' && cur <= ('0' + base - 1) ));
        else if (base == 16) count += (status = !!isxdigit(cur));
        else; /**/

        if(status == 0) break;
    }

    if(count == 0) return NULL;
    char* b = malloc(count + 1);
    strncpy(b, &input[basec], count);
    b[count] = 0;

    mpz_t result_int;
    mpz_init_set_str(result_int, b, base);

    LexerToken *result = create_filledtoken(TOKEN_INT, result_int, sizeof(result_int));
    result->original_tokenc = basec + count;

    return result;
}

LexerToken* tokenize_floatliteral(char *input)
{
    return NULL;
}

LexerToken* tokenize_operator(char *input)
{
    for(int i = 0; i  < (sizeof operators/sizeof *operators); ++i)
    {
        
        char *current = operators[i];
        size_t current_len = strlen(current);
        if(strncmp(current, input, current_len) == 0)
        {
            LexerToken* result = create_filledtoken(TOKEN_OPERATOR, current, current_len+1);
            ((char*)(result->value))[current_len] = 0;
            result->original_tokenc = current_len;
            return result;
        }
    }
    return NULL;
}

LexerToken* tokenize_parens(char* input)
{
    LexerToken *result;
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


char *tok2str(TokenType t)
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
        case METATOKEN_TERMINATE:
            return "terminate";
        default: return "????";
   } 
}
