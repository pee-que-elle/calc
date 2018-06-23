#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <gmp.h>

LexerToken* create_emptytoken(TokenType type)
{
    LexerToken *tok = malloc(sizeof(LexerToken));
    tok->type = type;
    tok->value = NULL;
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
    LexerToken* toks = malloc(0);
    size_t tokc = 0;

    for(int i = 0; i < strlen(input); ++i)
    {
        
    }
}


LexerToken* tokenize(char *input)
{

    #define CHECK(x)                        \
        result = x(input);                  \
        if(result != NULL) return result;
        
    LexerToken *result = NULL;
    
    CHECK(tokenize_stringliteral);
    CHECK(tokenize_intliteral);
    CHECK(tokenize_floatliteral);
    CHECK(tokenize_operator);
    
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
            return result;
        }         
    }
    return NULL; /* Reached end of input withput encountering a '"' */
}

LexerToken* tokenize_intliteral(char *input)
{
    #define CHECKBASE(c, b)                     \
        if(input[1] == b && input[0] == '0')    \
        {                                       \
            base = b;                           \
            i += 2;                             \
            goto endbasecheck;                  \
        } 
    int i = 0;
    int count = 0;
    int base = 10;
    
    CHECKBASE('x', 16);
    CHECKBASE('b', 2);
    CHECKBASE('o', 8);
    
    endbasecheck:
    while(1)
    {
        char c = input[i+count];
        int status = 0;

        if(base <= 10) count += (status = (c >= '0' && c <= ('0' + base - 1) ));
        else if (base == 16) count += (status = !!isxdigit(c));
        else; /**/

        if(status == 0) break;
    }

    if(count == 0) return NULL;
    char* b = malloc(count + 1);
    strncpy(b, &input[i], count);
    b[count] = 0;

    mpz_t result_int;
    mpz_init_set_str(result_int, b, base);

    LexerToken *result = create_filledtoken(TOKEN_INT, result_int, sizeof(result_int));
    return result;

}

LexerToken* tokenize_floatliteral(char *input)
{
    return NULL;
}

LexerToken* tokenize_operator(char *input)
{
    for(int i = 0; i  <= (sizeof operators/sizeof *operators); ++i)
    {
        char* current = operators[i];
        size_t current_len = strlen(operators[i]);
        if(strncmp(current, input, current_len) == 0)
        {
            LexerToken* result = create_filledtoken(TOKEN_OPERATOR, current, current_len+1);
            ((char*)(result->value))[current_len] = 0;
        }
    }
    return NULL;
}

LexerToken* tokenize_parens(char* input)
{
    return (*input == '(' ? (create_emptytoken(TOKEN_LPAREN)) : (*input == ')' ? (create_emptytoken(TOKEN_RPAREN)) : (NULL) ));
}
