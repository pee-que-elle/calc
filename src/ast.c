#include <stdlib.h>
#include <string.h>
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
    o->value = value;
    return ASTNode(NODE_STRING, o);
}

ASTNode_T *Identifier(char *value)
{
    Identifier_T *o = malloc(sizeof(Identifier_T));
    o->identifier = value;
    return ASTNode(NODE_IDENTIFIER, o);
}
