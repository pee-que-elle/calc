/* This file describes a list of generally immutable types used to represent
 * an abstract syntax tree. A parser will convert an expression to an AST 
 * made up of these nodes.
 */

#include <gmp.h> /* arbitrary precision numbers */
#include <stdarg.h> /* varargs */

/* Forward decls */
typedef struct ASTNode ASTNode_T;
    typedef struct Expression Expression_T;
    typedef struct FunctionCall FunctionCall_T;
    typedef struct Operator Operator_T;
        typedef struct UnaryOperator UnaryOperator_T;
        typedef struct BinaryOperator BinaryOperator_T;
    typedef struct AbstractValue AbstractValue_T;
        typedef struct Integer Integer_T;
        typedef struct Float Float_T;
        typedef struct String String_T;

typedef enum NodeType {
    NODE_EXPRESSION,
    NODE_FUNCTIONCALL,
    NODE_OPERATOR,
    NODE_ABSTRACTVALUE,
    NODE_NONE
} NodeType;
    typedef enum OperatorType {
        OPERATOR_UNARY,
        OPERATOR_BINARY,
        OPERATOR_NONE
    } OperatorType;
        typedef enum UnaryOperatorType {
            UNARYOPERATOR_NOT,
            UNARYOPERATOR_SIGN,
            UNARYOPERATOR_FACTORIAL,
            UNARYOPERATOR_NONE
        } UnaryOperatorType;
        typedef enum BinaryOperatorType { 
            BINARYOPERATOR_ADDITION, 
            BINARYOPERATOR_SUBTRACTION, 
            BINARYOPERATOR_MULTIPLICATION,
            BINARYOPERATOR_DIVISION, 
            BINARYOPERATOR_EXPONENTIATION,
            BINARYOPERATOR_MODULATION,
            BINARYOPERATOR_AND,
            BINARYOPERATOR_OR,
            BINARYOPERATOR_XOR,
            BINARYOPERATOR_NONE
        } BinaryOperatorType;
    typedef enum AbstractValueType {
        ABSTRACTVALUE_STRING,
        ABSTRACTVALUE_INTEGER,
        ABSTRACTVALUE_FLOAT,
        ABSTRACTVALUE_NONE
    } AbstractValueType;

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

struct Operator {
    OperatorType type;
    void *operator;
};

struct UnaryOperator {
    /* ~x, -x, +x, x! */
    UnaryOperatorType type;
    Expression_T *operand;
};

struct BinaryOperator {

    /* x + y, x - y, x * y, x ^ y, x ** y, etc */
    BinaryOperatorType type;
    Expression_T *lhand;
    Expression_T *rhand;
};

struct AbstractValue {
    AbstractValueType type;
    void *value;
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
    /* These generate the raw structs, not wrapped in an ASTNode */
    Expression_T *N_Expression(ASTNode_T **nodes);
    FunctionCall_T *N_FunctionCall(char* name, Expression_T **args);
    Operator_T *N_Operator(OperatorType type, void *operator);
        UnaryOperator_T *N_UnaryOperator(UnaryOperatorType type, Expression_T *operand);
        BinaryOperator_T *N_BinaryOperator(BinaryOperatorType type, Expression_T *lhand, Expression_T *rhand);    
    AbstractValue_T *N_AbstractValue(AbstractValueType type, void* value);
        Integer_T *N_Integer(mpz_t value);
        Float_T *N_Float(mpf_t value);
        String_T *N_String(char *value);

    /* These generate the wrapped versions */
    ASTNode_T *Expression(ASTNode_T **nodes);
    ASTNode_T *FunctionCall(char* name, Expression_T **args);
    ASTNode_T *Operator(OperatorType type, void *operator);
        ASTNode_T *UnaryOperator(UnaryOperatorType type, Expression_T *operand);
        ASTNode_T *BinaryOperator(BinaryOperatorType type, Expression_T *lhand, Expression_T *rhand);    
    ASTNode_T *AbstractValue(AbstractValueType type, void* value);
        ASTNode_T *Integer(mpz_t value);
        ASTNode_T *Float(mpf_t value);
        ASTNode_T *String(char *value);

