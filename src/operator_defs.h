#ifndef CALC_OPERATOR_DEFS
#define CALC_OPERATOR_DEFS

#define MERGE_ARRAYS(...) {__VA_ARGS__} 

#define UNARY_OPERS \
    "!", \
    "|", \
    "&", \
    "~"

#define BINARY_OPERS \
    "&&", \
    "||", \
    "<<", \
    ">>", \
    "==", \
    "!=", \
    "<=", \
    ">=", \
    "**", \
          \
    "*",  \
    "/",  \
    "%",  \
    "^",  \
    "<",  \
    ">"

#define AMBIGUOUS_OPERS \
    "+",  \
    "-"

/* will need a rewrite if we decide to add multi char unary operators lol */

#include "ast.h"

typedef struct Precedence {
    OperatorType t;
    int type;
    size_t precedence; 
    
} Precedence_T;

static char *unary_operators[] = MERGE_ARRAYS(UNARY_OPERS);
static char *binary_operators[] = MERGE_ARRAYS(BINARY_OPERS);
static char *ambiguous_operators[] = MERGE_ARRAYS(AMBIGUOUS_OPERS);
static char *operators[] = MERGE_ARRAYS(BINARY_OPERS, UNARY_OPERS, AMBIGUOUS_OPERS);

#define AP(type, type2, precedence) {type, type2, precedence}
#define APU(precedence, x) AP(OPERATOR_UNARY,  UNARYOPERATOR_##x, precedence)
#define APB(precedence, x) AP(OPERATOR_BINARY, BINARYOPERATOR_##x, precedence)

static Precedence_T operator_precedences[] = {
    APB(0, EXPONENTIATION),
    APU(0, FACTORIAL),
    
    APU(1, BITWISENOT),
    APU(1, LOGICALNOT),
    APU(1, SIGN),

    APB(2, MULTIPLICATION),
    APB(2, DIVISION),
    APB(2, MODULATION),

    APB(3, ADDITION),
    APB(3, SUBTRACTION),

    APB(4, BITWISELEFTSHIFT),
    APB(4, BITWISERIGHTSHIFT),
    
    APB(5, GREATERTHAN),
    APB(5, GREATERTHANOREQUAL),
    APB(5, LESSERTHAN),
    APB(5, LESSERTHANOREQUAL),

    APB(6, EQUALS),
    APB(6, NOTEQUALS),

    APB(7, BITWISEAND),
    
    APB(8, BITWISEXOR),

    APB(9, BITWISEOR),

    APB(10, LOGICALAND),

    APB(11, LOGICALXOR),

    APB(12, LOGICALOR)
};


#endif //CALC_OPERATOR_DEFS 
