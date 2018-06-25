#include "lexer.h"

#include "operator.h"

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
    strncpy(tok->value, value, n);
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
        #if IGNORE_WHITESPACE
            while(isspace(input[i]) && input[i] != 0) i++;
            if(input[i] == 0) break;
        #endif // IGNORE_WHITESPACE

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

    #undef REALLOC_TOKS
    #undef APPEND_TOKS
}

LexerToken* tokenize(char *input)
{

    #define CHECK(x)                        \
        result = x(input);                  \
        if(result != NULL) return result;
        
    LexerToken *result = NULL;
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
            result->value[len] = 0; /* null-terminate string properly */
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

        if(cur == 0) break; /* not technically needed but might save some headaches */
        int status = 0;

        if(base <= 10) count += (status = (cur >= '0' && cur <= ('0' + base - 1) ));
        else if (base == 16) count += (status = !!isxdigit(cur));
        else; /**/

        if(status == 0) break;
    }

    if(count == 0) return NULL;

    LexerToken *result = create_filledtoken(TOKEN_INT, input, basec+count+1);

    result->value[basec+count] = 0;

    result->original_tokenc = basec + count;

    return result;

    #undef CHECKBASE
}

LexerToken* tokenize_floatliteral(char *input)
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

    
    LexerToken *result = create_filledtoken(TOKEN_FLOAT, input, i+1);
    result->value[i] = 0;

    result->original_tokenc = i;

    return result;
}

LexerToken* tokenize_operator(char *input)
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
    LexerToken* result = create_filledtoken(TOKEN_OPERATOR, touse, touse_len+1);
    result->value[touse_len] = 0;
    result->original_tokenc = touse_len;
    return result;
}

LexerToken *tokenize_identifier(char *input)
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
    LexerToken *result = create_filledtoken(TOKEN_IDENTIFIER, input, i+1);
    result->value[i] = 0;
    result->original_tokenc = i;
    return result;
    
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
LexerToken* tokenize_comma(char *input)
{
    if(*input != ',') return NULL;

    LexerToken *result = create_emptytoken(TOKEN_COMMA);
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
        case METATOKEN_TERMINATE:
            return "terminate";
        default: return "????";
   } 
}

char *tok2str(LexerToken *tok)
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
