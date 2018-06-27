#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ASTNode_T *parse(LinkedList_T *ll_tokens)
{
    LexerToken_T **tokens = (LexerToken_T**)ll_to_array(ll_tokens);
    
    return fill_opers(parse_tokenstream(tokens));
    
}

ASTNode_T *fill_opers(ASTNode_T *tree)
{
    if(tree->type != NODE_EXPRESSION) return NULL;

    ASTNode_T **nodes = (ASTNode_T**)ll_to_array(((Expression_T*)(tree->assoc))->expr);

    for(int i = 0; nodes[i] != NULL; i++)
    {
        ASTNode_T *node = nodes[i];
        if(node->type == NODE_OPERATOR_PLACEHOLDER)
        {
            char *identifier = ((Operator_T*)(node->assoc))->identifier;
            
            LinkedList_T *matching_operators = match_operator_by_criteria(
                    identifier, 
                    -1,
                    OPERATOR_NONE,
                    OPERATORARITY_NONE,
                    0);

            size_t matchc = ll_size(matching_operators);

            if(matchc == 0)
            {
                printf("Something has gone wrong\n");
                return NULL;
            }
        }
    }
}

ASTNode_T *parse_tokenstream(LexerToken_T **tokens)
{
    /* Create non-nested AST for further evauluation */
    LinkedList_T *astnodes = ll_create_empty();
    LinkedList_T *current = astnodes;
    
    for(int i = 0; tokens[i] != NULL; ++i)
    {
       LexerToken_T *token = tokens[i]; 
       Operator_T *oper;
       switch(token->type)
       {
            case TOKEN_STRING:
                current = ll_append(current, ll_create_filled_nocopy(String(token->value), 1));
                break;
            case TOKEN_IDENTIFIER:
                current = ll_append(current, ll_create_filled_nocopy(Identifier(token->value), 1));
                break;

            case TOKEN_OPERATOR:
                /* Make new operator with only identifier left in. Fill in deatils later once complete context is built */
                oper = malloc(sizeof(Operator_T));
                oper->identifier = malloc(strlen(token->value + 1));
                strcpy(oper->identifier, token->value);

                current = ll_append(current, ll_create_filled_nocopy(ASTNode(NODE_OPERATOR_PLACEHOLDER, oper), 1));
                break; 
            case TOKEN_LPAREN:
                current = ll_append(current, ll_create_filled_nocopy(parse_tokenstream(&tokens[i+1]), 1));
                i += ll_size(current);
                break;
            case TOKEN_RPAREN:
                return Expression(astnodes);

            case TOKEN_COMMA:
                current = ll_append(current, ll_create_filled_nocopy(ASTNode(NODE_SEPERATOR_PLACEHOLDER, NULL), 1));
                break;

       }
    } 
    return Expression(astnodes);
}
