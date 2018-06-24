#ifndef CALC_LEXER
#define CALC_LEXER

#define IGNORE_WHITESPACE 1

#include "ast.h"


typedef enum TokenType {
    TOKEN_STRING,           /* String Literal */ 
    TOKEN_INT,              /* Int Literal */
    TOKEN_FLOAT,            /* Float Literal */
    TOKEN_OPERATOR,         /* Operator */
    TOKEN_LPAREN,           /* Left parenthesis, '(' */
    TOKEN_RPAREN,           /* Right parenthesis, ')' */
    METATOKEN_TERMINATE     /* Metatoken; Signifies end of token sequence */ 
} TokenType;

typedef struct LexerToken {
    TokenType type;
    void *value;
    size_t original_tokenc; /* Original token size */
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


char *tok2str(TokenType t);

#endif // CALC_LEXER
