#include "operator.h"
#include <stdlib.h>
#include <string.h>

LinkedList_T *match_operator_by_criteria(char *identifier, int precedence, OperatorType type, OperatorArity arity, int arity_specific_value)
{
    LinkedList_T *matches = ll_create_empty();
    
    LinkedList_T *current = matches;

    for(int i = 0; i < (sizeof operators / sizeof *operators); ++i)
    {
        Operator_T *currentoper = &operators[i];
        if(
                 (identifier == NULL          || strcmp(identifier, currentoper->identifier) == 0) 
              && (precedence == -1            || precedence == currentoper->precedence) 
              && (type == OPERATOR_NONE || type == currentoper->type) 
              && (arity == OPERATORARITY_NONE 
                  || ((arity == currentoper->arity) 
                      && ((arity == OPERATORARITY_UNARY && ((UnaryOperatorAffix)arity_specific_value == OPERATORAFFIX_NONE || (UnaryOperatorAffix)arity_specific_value == currentoper->affix))
                      || (arity == OPERATORARITY_BINARY && ((BinaryOperatorAssociativity)arity_specific_value == OPERATORASSOC_NONE || (BinaryOperatorAssociativity)arity_specific_value == currentoper->associativity)))
                 )
              )
          )
        {
            current = ll_append(ll_set_nocopy(current, currentoper, 0, sizeof(Operator_T)), ll_create_empty());
        }
    }
    return matches;
}
