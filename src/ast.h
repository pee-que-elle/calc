/* This file describes a list of generally immutable types used to represent
 * an abstract syntax tree. A parser will convert an expression to an AST 
 * made up of these nodes.
 */

#ifndef CALC_AST
#define CALC_AST

#include <gmp.h> /* arbitrary precision numbers */
#include <stdarg.h> /* varargs */

#include "operator.h"

/* Forward decls */
typedef struct ASTNode ASTNode_T;
    typedef struct Expression Expression_T;
    typedef struct FunctionCall FunctionCall_T;
    typedef struct UnaryOperator UnaryOperator_T;
    typedef struct BinaryOperator BinaryOperator_T;
    typedef struct Integer Integer_T;
    typedef struct Float Float_T;
    typedef struct String String_T;

typedef enum NodeType {
    NODE_EXPRESSION,
    NODE_FUNCTIONCALL,
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

struct Expression {
   ASTNode_T **expr; 
};

struct FunctionCall {
    char* name;
    Expression_T **args;
};

struct UnaryOperator {
    /* ~x, -x, +x, x!, etc */
    OperatorType type;
    Expression_T *operand;
};

struct BinaryOperator {
    /* x + y, x - y, x * y, x ^ y, x ** y, etc */
    OperatorType type;
    Expression_T *lhand;
    Expression_T *rhand;
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

ASTNode_T *ASTNode(NodeType type, void *assoc);
    ASTNode_T *Expression(ASTNode_T **nodes);
    ASTNode_T *FunctionCall(char* name, Expression_T **args);
    ASTNode_T *UnaryOperator(OperatorType type, Expression_T *operand);
    ASTNode_T *BinaryOperator(OperatorType type, Expression_T *lhand, Expression_T *rhand);    
    ASTNode_T *Integer(mpz_t value);
    ASTNode_T *Float(mpf_t value);
    ASTNode_T *String(char *value);
#endif // CALC_AST
