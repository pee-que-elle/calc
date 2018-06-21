#include "ast.h"


typedef enum TokenType { 
    STRINGLITERAL, 
    INTLITERAL,
    FLOATLITERAL,
    OPERATOR,
    FUNCTION
} TokenType;

typedef struct LexerToken {
    TokenType type;
    char *value;
} LexerToken;


LexerToken* create_token(TokenType Type, char* value);
LexerToken* lex(char *input);

LexerToken* tokenize(char *input);
    
    LexerToken* tokenize_stringliteral(char *input);
    LexerToken* tokenize_intliteral(char *input);
    LexerToken* tokenize_floatliteral(char *input);
    LexerToken* tokenize_operator(char *input);
    LexerToken* tokenize_function(char *input);
