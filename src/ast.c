#include <stdlib.h>
#include <string.h>
#include "ast.h"

#include "operator.h"

ASTNode_T *ASTNode(NodeType type, void *assoc)
{
    ASTNode_T *n = malloc(sizeof(ASTNode_T));
    n->type = type;
    n->assoc = assoc;
    return n;
}

ASTNode_T *UnaryOperator(OperatorType type, Expression_T *operand)
{

    UnaryOperator_T *o = malloc(sizeof(UnaryOperator_T));
    o->type = type;
    o->operand = operand;
    return ASTNode(NODE_UNARYOPERATOR, o);
}

ASTNode_T *BinaryOperator(OperatorType type, Expression_T *lhand, Expression_T *rhand)
{
    
    BinaryOperator_T *o = malloc(sizeof(BinaryOperator_T));
    o->type = type;
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
