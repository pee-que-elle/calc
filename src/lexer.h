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
} LexerToken_T;


LexerToken_T* create_emptytoken(TokenType type);
LexerToken_T* create_filledtoken(TokenType type, void *value, size_t n); 
LexerToken_T* lex(char *input);

LexerToken_T* tokenize(char *input);
    
    LexerToken_T* tokenize_stringliteral(char *input);
    LexerToken_T* tokenize_identifier(char *input);
    LexerToken_T* tokenize_intliteral(char *input);
    LexerToken_T* tokenize_floatliteral(char *input);
    LexerToken_T* tokenize_operator(char *input);
    LexerToken_T* tokenize_parens(char *input);
    LexerToken_T* tokenize_comma(char *input);
    LexerToken_T* tokenize_identifier(char *input);


char *toktype2str(TokenType t);
char *tok2str(LexerToken_T *tok);

#endif // CALC_LEXER
