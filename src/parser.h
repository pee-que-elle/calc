#ifndef CALC_PARSER
#define CALC_PARSER

#include "ast.h"
#include "lexer.h"
#include "linkedlist.h"

#include <gmp.h>

ASTNode_T *parse(LinkedList_T *tokens);

ASTNode_T *parse_atom(Lexer_T *lexer);
ASTNode_T *parse_expr(Lexer_T *lexer, int min_prec);

ASTNode_T *parse_binop(Operator_T *op, ASTNode_T *lhand, ASTNode_T *rhand);
ASTNode_T *parse_unop(Operator_T *op, ASTNode_T *oper);

Operator_T *deduce_operator_from_context(char *identifier, LexerToken_T *next);

void tokenint2mpz(mpz_t r, char *str);
void tokenfloat2mpf(mpf_t r, char *str);
#endif //CALC_PARSER
