#ifndef CALC_LEXER
#define CALC_LEXER

#define IGNORE_WHITESPACE 1

#include "ast.h"
#include "linkedlist.h"

typedef enum TokenType {
    TOKEN_STRING,           /* String Literal */ 
    TOKEN_INT,              /* Int Literal */
    TOKEN_FLOAT,            /* Float Literal */
    TOKEN_OPERATOR,         /* Operator */
    TOKEN_LPAREN,           /* Left parenthesis, '(' */
    TOKEN_RPAREN,           /* Right parenthesis, ')' */
    TOKEN_COMMA,            /* Comma; ',' */
    TOKEN_IDENTIFIER       /* identifier (function name, variable name, etc) */
} TokenType;

typedef struct LexerToken {
    TokenType type;
    char *value;
    size_t original_tokenc; /* Original token size */
} LexerToken_T;

typedef struct Lexer {
    LinkedList_T/*<LexerToken_T*>*/ *current;
} Lexer_T;

LexerToken_T* create_emptytoken(TokenType type);
LexerToken_T* create_filledtoken(TokenType type, char *value, size_t n); 

LinkedList_T* lex(char *input);

LexerToken_T* tokenize(char *input);
    
    LexerToken_T* tokenize_stringliteral(char *input);
    LexerToken_T* tokenize_identifier(char *input);
    LexerToken_T* tokenize_intliteral(char *input);
    LexerToken_T* tokenize_floatliteral(char *input);
    LexerToken_T* tokenize_operator(char *input);
    LexerToken_T* tokenize_parens(char *input);
    LexerToken_T* tokenize_comma(char *input);
    LexerToken_T* tokenize_identifier(char *input);

void lexer_advance(Lexer_T *lexer);
LexerToken_T *lexer_current(Lexer_T *lexer);

char *toktype2str(TokenType t);
char *tok2str(LexerToken_T *tok);

/* Define both to prevent bugs in the future */
int tokisoperatable(TokenType t);
int tokisnotoperatable(TokenType t);

#endif // CALC_LEXER
