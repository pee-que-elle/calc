#ifndef CALC_OPERATOR
#define CALC_OPERATOR

#include "linkedlist.h"

typedef enum OperatorArity {
    OPERATORARITY_UNARY,
    OPERATORARITY_BINARY,
    OPERATORARITY_NONE
} OperatorArity;

typedef enum OperatorType {
    OPERATOR_BITWISENOT,
    OPERATOR_LOGICALNOT,
    OPERATOR_SIGNPOSITIVE,
    OPERATOR_SIGNNEGATIVE,
    OPERATOR_FACTORIAL,

    /* Arithmetic */
    OPERATOR_ADDITION, 
    OPERATOR_SUBTRACTION, 
    OPERATOR_MULTIPLICATION,
    OPERATOR_DIVISION, 
    OPERATOR_EXPONENTIATION,
    OPERATOR_MODULATION,

    /* Bitwise */
    OPERATOR_BITWISEAND,
    OPERATOR_BITWISEOR,
    OPERATOR_BITWISEXOR,
    OPERATOR_BITWISELEFTSHIFT,
    OPERATOR_BITWISERIGHTSHIFT,

    /* Logical */
    OPERATOR_LOGICALAND,
    OPERATOR_LOGICALOR,
    OPERATOR_LOGICALXOR,
            
    /* Comparison */
    OPERATOR_EQUALS,
    OPERATOR_NOTEQUALS,
    OPERATOR_GREATERTHAN,
    OPERATOR_GREATERTHANOREQUALS,
    OPERATOR_LESSERTHAN,
    OPERATOR_LESSERTHANOREQUALS,

    OPERATOR_NONE
} OperatorType;

typedef enum UnaryOperatorAffix {
    OPERATORAFFIX_PREFIX,
    OPERATORAFFIX_POSTFIX,
    OPERATORAFFIX_NONE,
} UnaryOperatorAffix;

typedef enum BinaryOperatorAssociativity {
    OPERATORASSOC_RIGHT,
    OPERATORASSOC_LEFT,
    OPERATORASSOC_NONE

} BinaryOperatorAssociativity;

typedef struct Operator {

    char *identifier;
    int precedence;
    OperatorType type;
    OperatorArity arity;
    union 
    {
        BinaryOperatorAssociativity associativity;
        UnaryOperatorAffix affix;
    };

} Operator_T;

#define O(i,p,t,o,a) {i, p, OPERATOR_##t, OPERATORARITY_##o, a}

#define RIGHT OPERATORASSOC_RIGHT
#define LEFT OPERATORASSOC_LEFT
#define PREFIX OPERATORAFFIX_PREFIX
#define POSTFIX OPERATORAFFIX_POSTFIX

static Operator_T operators[] = {

    O("**", 0 , EXPONENTIATION     , BINARY, RIGHT),
    O("!" , 0 , FACTORIAL          , UNARY , POSTFIX),
    O("~" , 1 , BITWISENOT         , UNARY , PREFIX),
    O("!" , 1 , LOGICALNOT         , UNARY , PREFIX),
    O("+" , 1 , SIGNPOSITIVE       , UNARY , PREFIX),
    O("-" , 1 , SIGNNEGATIVE       , UNARY , PREFIX),
    O("*",  2 , MULTIPLICATION     , BINARY, LEFT),
    O("/",  2 , DIVISION           , BINARY, LEFT),
    O("%",  2 , MODULATION         , BINARY, LEFT),
    O("+",  3 , ADDITION           , BINARY, LEFT),
    O("-",  3 , SUBTRACTION        , BINARY, LEFT),
    O(">>", 4 , BITWISELEFTSHIFT   , BINARY, LEFT),
    O("<<", 4 , BITWISERIGHTSHIFT  , BINARY, LEFT),
    O(">",  5 , GREATERTHAN,         BINARY, LEFT),
    O(">=", 5 , GREATERTHANOREQUALS, BINARY, LEFT),
    O("<",  5 , LESSERTHAN         , BINARY, LEFT),
    O("<=", 5 , LESSERTHANOREQUALS , BINARY, LEFT),
    O("==", 6 , EQUALS             , BINARY, LEFT),
    O("!=", 6 , NOTEQUALS          , BINARY, LEFT),
    O("&",  7 , BITWISEAND         , BINARY, LEFT),
    O("^",  8 , BITWISEXOR         , BINARY, LEFT),
    O("|",  9 , BITWISEOR          , BINARY, LEFT),
    O("&&", 10, LOGICALAND         , BINARY, LEFT),
    O("^^", 11, LOGICALXOR         , BINARY, LEFT),
    O("||", 12, LOGICALOR          , BINARY, LEFT)
};


LinkedList_T *match_operator_by_criteria(char *identifier, int precedence, OperatorType type, OperatorArity arity, int arity_specific_value);

#endif //CALC_OPERATOR
