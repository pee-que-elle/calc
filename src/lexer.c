#include "lexer.h"
#include <stdlib.h>
#include <string.h>

LexerToken* create_token(TokenType type, char* value)
{
    LexerToken *tok = malloc(sizeof(LexerToken));
    tok->type = type;
    strcpy(tok->value, value);
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

}

LexerToken* tokenize_stringliteral(char *input)
{

}

LexerToken* tokenize_intliteral(char *input)
{

}

LexerToken* tokenize_floatliteral(char *input)
{

}

LexerToken* tokenize_operator(char *input)
{

}

LexerToken* tokenize_function(char *input)
{

}
