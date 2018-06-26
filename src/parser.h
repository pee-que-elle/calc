#ifndef CALC_PARSER
#define CALC_PARSER

#include "ast.h"
#include "lexer.h"
#include "linkedlist.h"

ASTNode_T *parse_tokenstream(LinkedList_T *tokens);


#endif //CALC_PARSER
