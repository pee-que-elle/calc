#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>


char *parsererror2str(ParserError e)
{
    #define ERRORCASE(error, string) case PARSERERROR_##error: return string;

    switch(e)
    {
        ERRORCASE(UNBALANCEDPARENTHESES, "Unbalanced parentheses encountered");
        default: return "???";
    }
}

ASTNode_T *parse(LinkedList_T *tokens)
{
    Lexer_T *lexer = malloc(sizeof(Lexer_T));
    lexer->current = tokens;
    
    ASTNode_T *result = parse_expr(lexer, INT_MAX);
    free(lexer);
    return result;
}

ASTNode_T *parse_atom(Lexer_T *lexer)
{
    LexerToken_T *cur = lexer_current(lexer);
    
    ASTNode_T *result = NULL;

    if(cur == NULL) return NULL;
    if(cur->type == TOKEN_LPAREN)
    {
        lexer_advance(lexer);
        result = parse_expr(lexer,  INT_MAX);
        
        if(result == NULL) return NULL;

        cur = lexer_current(lexer);
        if(cur == NULL || cur->type != TOKEN_RPAREN)
        {
            puts(parsererror2str(PARSERERROR_UNBALANCEDPARENTHESES));
            return NULL;
        }
        lexer_advance(lexer);
        
    }
    else if(cur->type == TOKEN_IDENTIFIER)
    {
        ASTNode_T *identifier = Identifier(cur->value);
        lexer_advance(lexer);
         
        cur = lexer_current(lexer);

        if(cur != NULL && cur->type == TOKEN_LPAREN)
        {
            lexer_advance(lexer);
            
            LinkedList_T *params = ll_create_empty();
            LinkedList_T *current_param = params;

            while(1)
            {
                ASTNode_T *cur_node = parse_expr(lexer, INT_MAX);
                if(cur_node == NULL)
                { 
                    puts(parsererror2str(PARSERERROR_UNBALANCEDPARENTHESES));
                    return NULL;
                }
                ll_set_nocopy(current_param, cur_node, 1, sizeof(ASTNode_T));
                if(current_param->value == NULL) return NULL;

                current_param = ll_append(current_param, ll_create_empty());
                cur = lexer_current(lexer);
            
                if(cur == NULL)
                {
                    puts(parsererror2str(PARSERERROR_UNBALANCEDPARENTHESES));
                    return NULL;
                }

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
            result = FunctionCall(identifier, params);
        }

        else
        {
            result = identifier;
        }
    }
    else if(cur->type == TOKEN_STRING)
    {
        lexer_advance(lexer);
        result = String(cur->value);
    }

    else if(cur->type == TOKEN_FLOAT)
    {
        lexer_advance(lexer);
        mpf_t f;
        mpf_init(f);
        tokenfloat2mpf(f, cur->value);
        result =  Float(f);
        mpf_clear(f);
    }

    else if(cur->type == TOKEN_INT)
    {
        lexer_advance(lexer);
        mpz_t i;
        mpz_init(i);
        tokenint2mpz(i, cur->value);

        result = Integer(i);

        mpz_clear(i);
    }

    else if(cur->type == TOKEN_OPERATOR)
    {
        lexer_advance(lexer);
        LexerToken_T *next = lexer_current(lexer);


        if(!(tokisoperatable(next->type) || next->type == TOKEN_LPAREN)) return NULL;

        LinkedList_T *ops = match_operator_by_criteria(cur->value, -1, OPERATOR_NONE, OPERATORARITY_UNARY, OPERATORAFFIX_PREFIX);

        if(ll_size(ops) != 1) return NULL;
        
        ASTNode_T *operand = parse_atom(lexer);

        result = parse_unop((Operator_T*)ops->value, operand);
    }
    
    return result;
}

ASTNode_T *parse_expr(Lexer_T *lexer, int max_prec)
{

    ASTNode_T *lhand = parse_atom(lexer);
    if(lhand == NULL) return NULL;

    ASTNode_T *rhand = NULL;

    size_t next_max_prec = max_prec;
    
    while(1)
    {
        LexerToken_T *cur = lexer_current(lexer);

        if(cur == NULL || cur->type != TOKEN_OPERATOR) break;
        
        LinkedList_T *operators = match_operator_by_criteria(cur->value, -1, OPERATOR_NONE, OPERATORARITY_BINARY, OPERATORASSOC_NONE);
        if(ll_size(operators) != 1) return NULL;
        
        Operator_T *op = operators->value;
        
        ll_free(operators, free); 
        if(op->precedence > max_prec) break;
                
        next_max_prec = op->precedence;

        if(op->associativity == OPERATORASSOC_LEFT) next_max_prec -= 1;

        lexer_advance(lexer);
        rhand = parse_expr(lexer, next_max_prec);
        if(rhand == NULL) return NULL;

        lhand = parse_binop(op, lhand, rhand);        

    }

    return lhand;

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
