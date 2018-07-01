#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "ast.h"
#include "linkedlist.h"
#include "operator.h"

ASTNode_T *ASTNode(NodeType type, void *assoc)
{
    ASTNode_T *n = malloc(sizeof(ASTNode_T));
    n->type = type;
    n->assoc = assoc;
    return n;
}

ASTNode_T *FunctionCall(ASTNode_T *identifier, LinkedList_T *args)
{
    FunctionCall_T *f = malloc(sizeof(BinaryOperator_T));
    f->identifier = identifier;
    f->args = args;
    return ASTNode(NODE_FUNCTIONCALL, f);
}

ASTNode_T *UnaryOperator(Operator_T *operator, ASTNode_T *operand)
{

    UnaryOperator_T *o = malloc(sizeof(UnaryOperator_T));
    o->operator = operator;
    o->operand = operand;
    return ASTNode(NODE_UNARYOPERATOR, o);
}

ASTNode_T *BinaryOperator(Operator_T *operator, ASTNode_T *lhand, ASTNode_T *rhand)
{
    
    BinaryOperator_T *o = malloc(sizeof(BinaryOperator_T));
    o->operator = operator;
    o->lhand = lhand;
    o->rhand = rhand;
    return ASTNode(NODE_BINARYOPERATOR, o);
}

ASTNode_T *Integer(mpz_t value)
{
    Integer_T *o = malloc(sizeof(Integer_T));

    mpz_init(o->value);
    mpz_set(o->value, value);
    return ASTNode(NODE_INTEGER, o);
}

ASTNode_T *Float(mpf_t value)
{
    Float_T *o = malloc(sizeof(Float_T));

    mpf_init(o->value);
    mpf_set(o->value, value);
    return ASTNode(NODE_FLOAT, o);
}

ASTNode_T *String(char *value)
{
    String_T *o = malloc(sizeof(String_T));
    o->value = malloc(strlen(value)+1);
    strcpy(o->value, value);
    return ASTNode(NODE_STRING, o);
}

ASTNode_T *Identifier(char *value)
{
    Identifier_T *o = malloc(sizeof(Identifier_T));
    o->identifier = value;
    return ASTNode(NODE_IDENTIFIER, o);
}

char *nodetype2str(NodeType t)
{
    #define CASE_STR(t, x) case NODE_##t: return x;

    switch(t)
    {
        CASE_STR(FUNCTIONCALL, "functioncall");
        CASE_STR(IDENTIFIER, "identifier");
        CASE_STR(UNARYOPERATOR, "unaryoperator");
        CASE_STR(BINARYOPERATOR, "binaryoperator");
        CASE_STR(STRING, "string");
        CASE_STR(INTEGER, "integer");
        CASE_STR(FLOAT, "float");
        CASE_STR(NONE, "none");
        default: return "????";
    }

#undef CASE_STR
}


char *ast2str(ASTNode_T *str)
{

    char *result = NULL;

    char *format = "%s(%s)";
    asprintf(&format, format, nodetype2str(str->type), "%s");
    format[0] = toupper(format[0]);
    char *subformat = NULL;
    
    char *str_accumulator[4] = {}; // THIS FUCKS OVER EVERYTHING. 
    
    //char **str_accumulator = malloc(sizeof(char*)*4);
    //memset(str_accumulator, 0,  sizeof(char*)*4);
    
    LinkedList_T *ll_accumulator = NULL;
    size_t size_accumulator = 0;
    

    switch(str->type)
    {
        case NODE_FUNCTIONCALL:

            subformat = "identifier=%s, args=[%s]";

            asprintf(&result, format, subformat);
            
            str_accumulator[3] = result;

            ll_accumulator = ((FunctionCall_T*)(str->assoc))->args;
            
            str_accumulator[0] = ast2str(((FunctionCall_T*)(str->assoc))->identifier);
            
            str_accumulator[1] = malloc(1);
            str_accumulator[1][0] = 0;

            while(ll_accumulator->value != NULL)
            {
                printf("MEME: %s\n", str_accumulator[1]);
                str_accumulator[2] = ast2str(((ASTNode_T*)(ll_accumulator->value)));

                size_accumulator += ll_accumulator->next->value == NULL ? strlen(str_accumulator[2]) : strlen(str_accumulator[2]) + 2;

                str_accumulator[1] = realloc(str_accumulator[1], size_accumulator + 1);
                if(str_accumulator[1][0] == 0) strcpy(str_accumulator[1], str_accumulator[2]);
                else strcat(str_accumulator[1], str_accumulator[2]);

                if(ll_accumulator->next->value != NULL) strcat(str_accumulator[1], ", ");

                ll_accumulator = ll_accumulator->next;
            }
            asprintf(&result, result, str_accumulator[0], str_accumulator[1]);

            break;

        case NODE_INTEGER:
            gmp_asprintf(&result, "value=%Zd", ((Integer_T*)(str->assoc))->value);
            str_accumulator[0] = result;
            asprintf(&result, format, result);
            break;
        case NODE_FLOAT:
            gmp_asprintf(&result, "value=%.*F", 16, ((Float_T*)(str->assoc))->value);
            str_accumulator[0] = result;
            asprintf(&result, format, result);
            break;
        case NODE_STRING:
        case NODE_IDENTIFIER:
            asprintf(&result, "value=\"%s\"", ((String_T*)(str->assoc))->value);
            str_accumulator[0] = result;
            asprintf(&result, format, result);
            break;

        case NODE_BINARYOPERATOR:
            #define BINOP ((BinaryOperator_T*)(str->assoc))
            subformat = "operator='%s', lhand=%s, rhand=%s";
            
            str_accumulator[0] = ast2str(BINOP->lhand);
            str_accumulator[1] = ast2str(BINOP->rhand);

            asprintf(&str_accumulator[2], subformat, BINOP->operator->identifier, str_accumulator[0], str_accumulator[1]);
            asprintf(&result, format, str_accumulator[2]);
            break;
            
            #undef BINOP
       case NODE_UNARYOPERATOR:
            #define UNOP ((UnaryOperator_T*)(str->assoc))

            subformat = "operator='%s', operand=%s";
            
            str_accumulator[0] = ast2str(UNOP->operand);
            asprintf(&str_accumulator[1], subformat, UNOP->operator->identifier, str_accumulator[0]);

            asprintf(&result, format, str_accumulator[1]);
            break;

            #undef UNOP

    }
    free(str_accumulator[0]);
    free(str_accumulator[1]);
    free(str_accumulator[2]);
    free(str_accumulator[3]);
    return result;
}

void ast_free(ASTNode_T *t)
{
    #define C(x) ((x*)(t->assoc))
    
    LinkedList_T *current = NULL;

    switch(t->type)
    {
        case NODE_INTEGER:
            mpz_clear(C(Integer_T)->value);
            break;
        case NODE_FLOAT:
            mpf_clear(C(Float_T)->value);
            break;
        case NODE_STRING:
            free(C(String_T)->value);
            break;
        case NODE_FUNCTIONCALL:
            ast_free(C(FunctionCall_T)->identifier); 
            ll_free(C(FunctionCall_T)->args);
    }
    #undef C
}
