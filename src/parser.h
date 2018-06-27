#ifndef CALC_PARSER
#define CALC_PARSER

#include "ast.h"
#include "lexer.h"
#include "linkedlist.h"

ASTNode_T *parse(LinkedList_T *ll_tokens);
    ASTNode_T *parse_tokenstream(LexerToken_T **tokens);
    ASTNode_T *fill_opers(ASTNode_T *tree);

#endif //CALC_PARSER
