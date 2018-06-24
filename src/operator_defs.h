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

static char *unary_operators[] = MERGE_ARRAYS(UNARY_OPERS);
static char *binary_operators[] = MERGE_ARRAYS(BINARY_OPERS);
static char *ambiguous_operators[] = MERGE_ARRAYS(AMBIGUOUS_OPERS);
static char *operators[] = MERGE_ARRAYS(BINARY_OPERS, UNARY_OPERS, AMBIGUOUS_OPERS);

#endif // CALC_OPERATOR_DEFS
