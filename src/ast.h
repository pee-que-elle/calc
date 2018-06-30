/* This file describes a list of generally immutable types used to represent
 * an abstract syntax tree. A parser will convert an expression to an AST 
 * made up of these nodes.
 */

#ifndef CALC_AST
#define CALC_AST

#include <gmp.h> /* arbitrary precision numbers */
#include <stdarg.h> /* varargs */

#include "operator.h"
#include "linkedlist.h"

/* Forward decls */
typedef struct ASTNode ASTNode_T;
    typedef struct FunctionCall FunctionCall_T;
    typedef struct UnaryOperator UnaryOperator_T;
    typedef struct BinaryOperator BinaryOperator_T;
    typedef struct Integer Integer_T;
    typedef struct Float Float_T;
    typedef struct String String_T;
    typedef struct Identifier Identifier_T;

typedef enum NodeType {
    NODE_FUNCTIONCALL,
    NODE_IDENTIFIER,
    NODE_UNARYOPERATOR,
    NODE_BINARYOPERATOR,
    NODE_STRING,
    NODE_INTEGER,
    NODE_FLOAT,
    NODE_NONE
} NodeType;

struct ASTNode {
    NodeType type;
    void *assoc;
};

struct FunctionCall {
    ASTNode_T *identifier;
    LinkedList_T/*<ASTNode_T*>*/ *args;
};

struct UnaryOperator {
    /* ~x, -x, +x, x!, etc */
    Operator_T *operator;
    ASTNode_T *operand;
};

struct BinaryOperator {
    /* x + y, x - y, x * y, x ^ y, x ** y, etc */
    Operator_T *operator;
    ASTNode_T *lhand;
    ASTNode_T *rhand;
};

struct Integer {
    mpz_t value;
};
struct Float {
    mpf_t value;
};
struct String {
    char *value;
};

struct Identifier {
    char *identifier;
};

ASTNode_T *ASTNode(NodeType type, void *assoc);
    ASTNode_T *FunctionCall(ASTNode_T *identifier, LinkedList_T *args);
    ASTNode_T *UnaryOperator(Operator_T *operator, ASTNode_T *operand);
    ASTNode_T *BinaryOperator(Operator_T *operator, ASTNode_T *lhand, ASTNode_T *rhand);    
    ASTNode_T *Integer(mpz_t value);
    ASTNode_T *Float(mpf_t value);
    ASTNode_T *String(char *value);
    ASTNode_T *Identifier(char *value);
#endif // CALC_AST
