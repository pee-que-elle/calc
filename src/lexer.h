#include "ast.h"

char *operators[] = {
    "**",
    "*",
    "/",
    "+",
    "-",
    "%"
    "&",
    "|",
    "^",
    "~",
    "!"
};

typedef enum TokenType { 
    TOKEN_STRING, 
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_OPERATOR,
    TOKEN_LPAREN,
    TOKEN_RPAREN
} TokenType;

typedef struct LexerToken {
    TokenType type;
    void *value;
} LexerToken;


LexerToken* create_emptytoken(TokenType type);
LexerToken* create_filledtoken(TokenType type, void *value, size_t n); 
LexerToken* lex(char *input);

LexerToken* tokenize(char *input);
    
    LexerToken* tokenize_stringliteral(char *input);
    LexerToken* tokenize_identifier(char *input);
    LexerToken* tokenize_intliteral(char *input);
    LexerToken* tokenize_floatliteral(char *input);
    LexerToken* tokenize_operator(char *input);
    LexerToken* tokenize_parens(char *input);
