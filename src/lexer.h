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
    TOKEN_COMMA,            /* Comma; ',' */
    TOKEN_IDENTIFIER,       /* identifier (function name, variable name, etc) */
    METATOKEN_TERMINATE     /* Metatoken; Signifies end of token sequence. might replace with linked list so no need for terminators anymore then */ 
} TokenType;

typedef struct LexerToken {
    TokenType type;
    char *value;
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
    LexerToken* tokenize_comma(char *input);
    LexerToken* tokenize_identifier(char *input);


char *toktype2str(TokenType t);
char *tok2str(LexerToken *tok);

#endif // CALC_LEXER
