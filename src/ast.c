#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode_T *ASTNode(NodeType type, void *assoc)
{
    ASTNode_T *n = malloc(sizeof(ASTNode_T));
    n->type = type;
    n->assoc = assoc;
    return n;
}

Expression_T *N_Expression(ASTNode_T **expr)
{
    Expression_T *e = malloc(sizeof(Expression_T));
    e->expr = expr;
    return e;
}

FunctionCall_T *N_FunctionCall(char* name, Expression_T **args)
{
    FunctionCall_T *c = malloc(sizeof(FunctionCall_T));
    strcpy(c->name, name);
    c->args = args;
    return c;
}

Operator_T *N_Operator(OperatorType type, void *operator)
{
    Operator_T *o = malloc(sizeof(Operator_T));
    o->type = type;
    o->operator = operator;
    return o;
}

UnaryOperator_T *N_UnaryOperator(UnaryOperatorType type, Expression_T *operand)
{
    UnaryOperator_T *o = malloc(sizeof(UnaryOperator_T));
    o->type = type;
    o->operand = operand;
    return o;
}

BinaryOperator_T *N_BinaryOperator(BinaryOperatorType type, Expression_T *lhand, Expression_T *rhand)
{
    BinaryOperator_T *o = malloc(sizeof(BinaryOperator_T));
    o->type = type;
    o->lhand = lhand;
    o->rhand = rhand;
    return o;
}

AbstractValue_T *N_AbstractValue(AbstractValueType type, void* value)
{
    AbstractValue_T *v = malloc(sizeof(AbstractValue_T));
    v->type = type;
    v->value = value;
    return v;
}

Integer_T *N_Integer(mpz_t value)
{
    Integer_T *i = malloc(sizeof(Integer_T));
    mpz_set(i->value, value);
    return i;
}

Float_T *N_Float(mpf_t value)
{
    Float_T *f = malloc(sizeof(Integer_T));
    mpf_set(f->value, value);
    return f;
}

String_T *N_String(char *value)
{
    String_T *s = malloc(sizeof(String_T));

    s->value = malloc(strlen(value)+1);
    strcpy(value, s->value);
    return s;
}

ASTNode_T *Operator(OperatorType type, void *operator)
{
    return ASTNode(NODE_OPERATOR, N_Operator(type, operator));
}


ASTNode_T *UnaryOperator(UnaryOperatorType type, Expression_T *operand)
{
    return Operator(OPERATOR_UNARY, N_UnaryOperator(type, operand));
}

ASTNode_T *BinaryOperator(BinaryOperatorType type, Expression_T *lhand, Expression_T *rhand)
{
    return Operator(OPERATOR_BINARY, N_BinaryOperator(type, lhand, rhand));
}

ASTNode_T *AbstractValue(AbstractValueType type, void *value) 
{
    return ASTNode(NODE_ABSTRACTVALUE, N_AbstractValue(type, value));
}

ASTNode_T *Integer(mpz_t value)
{
    return AbstractValue(ABSTRACTVALUE_INTEGER, N_Integer(value));
}

ASTNode_T *Float(mpf_t value)
{
    return AbstractValue(ABSTRACTVALUE_FLOAT, N_Float(value));
}

ASTNode_T *String(char *value)
{
    return AbstractValue(ABSTRACTVALUE_STRING, N_String(value));
}
