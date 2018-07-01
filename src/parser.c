#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>

ASTNode_T *parse(LinkedList_T *tokens)
{
    Lexer_T *lexer = malloc(sizeof(Lexer_T));
    lexer->current = tokens;
    
    return parse_expr(lexer, 2147483647);
}

ASTNode_T *parse_atom(Lexer_T *lexer)
{
    LexerToken_T *cur = lexer_current(lexer);
    
    if(cur->type == TOKEN_LPAREN)
    {
        lexer_advance(lexer);

    }
    else if(cur->type == TOKEN_IDENTIFIER)
    {
        ASTNode_T *identifier = Identifier(cur->value);
        lexer_advance(lexer);
           
        if(lexer_current(lexer)->type == TOKEN_LPAREN)
        {
            lexer_advance(lexer);
            
            LinkedList_T *params = ll_create_empty();
            LinkedList_T *current_param = params;

            while(1)
            {
                ll_set_nocopy(current_param, parse_expr(lexer, 1), 1, sizeof(ASTNode_T));
                
                if(current_param->value == NULL) return NULL;

                current_param = ll_append(current_param, ll_create_empty());
                cur = lexer_current(lexer);

                if(cur->type == TOKEN_COMMA)
                {
                    lexer_advance(lexer);
                }
                else if(cur->type == TOKEN_RPAREN)
                {
                    break;
                }
                else
                {
                    printf("Unexpected token '%s'", toktype2str(cur->type));
                    return NULL;
                }
            }
            return FunctionCall(identifier, params);
        }

        else
        {
            return identifier;
        }
    }
    else if(cur->type == TOKEN_STRING)
    {
        lexer_advance(lexer);
        return String(cur->value);
    }

    else if(cur->type == TOKEN_FLOAT)
    {
        lexer_advance(lexer);
        mpf_t f;
        mpf_init(f);
        tokenfloat2mpf(f, cur->value);
        return Float(f);
    }

    else if(cur->type == TOKEN_INT)
    {
        lexer_advance(lexer);
        mpz_t i;
        mpz_init(i);
        tokenint2mpz(i, cur->value);
        return Integer(i);
    }
}

ASTNode_T *parse_expr(Lexer_T *lexer, int max_prec)
{
    LexerToken_T *prev = lexer_current(lexer);

    ASTNode_T *lhand = parse_atom(lexer);
    
    ASTNode_T *rhand = NULL;

    size_t next_max_prec = max_prec;
    

    while(1)
    {
        LexerToken_T *cur = lexer_current(lexer);
        if(cur == NULL || cur->type != TOKEN_OPERATOR) break;
        
        lexer_advance(lexer);
        LexerToken_T *next = lexer_current(lexer);
        
        Operator_T *op = deduce_operator_from_context(prev, cur->value, next);
        
        if(op->precedence > max_prec) break;

        if(op->arity == OPERATORARITY_UNARY)
        {
            
        }

        else if(op->arity == OPERATORARITY_BINARY)
        {
            next_max_prec = op->precedence;
            if(op->associativity == OPERATORASSOC_LEFT) next_max_prec -= 1;
            rhand = parse_expr(lexer, next_max_prec);
            lhand = parse_binop(op, lhand, rhand);
        }        
    }

    return lhand;

}

Operator_T *deduce_operator_from_context(LexerToken_T *prev_token, char *identifier, LexerToken_T *next_token)
{
    LinkedList_T *operators = match_operator_by_criteria(identifier, -1, OPERATOR_NONE, OPERATORARITY_NONE, 0);
   
    size_t opc = ll_size(operators);

    if(opc == 1) return (Operator_T*)operators->value;
    
    LinkedList_T *current = operators;
 
    while(current->value != NULL)
    {
        Operator_T *current_operator = (Operator_T*)current->value;

        if(current_operator->arity == OPERATORARITY_UNARY)
        {
            if(current_operator->affix == OPERATORAFFIX_PREFIX)
            {
               if(
                       (
                       tokisoperatable(next_token->type)
                       || next_token->type == TOKEN_LPAREN 
                       )
                       && 
                       (
                       tokisnotoperatable(prev_token->type)
                       || prev_token->type == TOKEN_LPAREN
                       )
                 ) return current_operator;
            }

            else if(current_operator->affix == OPERATORAFFIX_POSTFIX)
            {
                if(
                        (
                        tokisoperatable(prev_token->type)
                        || prev_token->type == TOKEN_RPAREN
                        )
                        &&
                        (
                        tokisnotoperatable(next_token->type)
                        || next_token->type == TOKEN_RPAREN
                        )

                  ) return current_operator;
            }
        }

        else if(current_operator->arity == OPERATORARITY_BINARY)
        {
            if(
                    (
                    tokisoperatable(next_token->type) 
                    || next_token->type == TOKEN_LPAREN
                    )
                    &&
                    (
                    tokisoperatable(prev_token->type)
                    || prev_token->type == TOKEN_RPAREN
                    )
              ) return current_operator;
        }
        
        current = current->next;
    }
    return NULL; 
}

ASTNode_T *parse_binop(Operator_T *op, ASTNode_T *lhand, ASTNode_T *rhand)
{
    return BinaryOperator(op, lhand, rhand);
}

ASTNode_T *parse_unop(Operator_T *op, ASTNode_T *operand)
{
    return UnaryOperator(op, operand);
}

void tokenint2mpz(mpz_t r, char *str)
{
    /* assume initialized mpz_t */
    int base = 0;

    if(strlen(str) <= 2)
    {
        base = 10;
    }

    else
    {
        switch(str[1])
        {
            case 'x':
                base = 16;
                break;
            case 'b':
                base = 2;
                break;
            case 'o':
                base = 8;
                break;
            default: 
                base = 10;
                break;
        }
    }
    mpz_set_str(r, base == 10 ? str : &str[2], base);
    
}

void tokenfloat2mpf(mpf_t r, char *str)
{
    mpf_set_str(r, str, 10);
}
