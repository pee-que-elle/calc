#include "operator.h"
#include <stdlib.h>
#include <string.h>

LinkedList_T *match_operator_by_criteria(char *identifier, int precedence, OperatorType type, OperatorArity arity, int arity_specific_value)
{
    LinkedList_T *matches = create_emptyll();
    
    LinkedList_T *current = matches;

    for(int i = 0; i < (sizeof operators / sizeof *operators); ++i)
    {
        Operator_T *currentoper = &operators[i];
        if(
              (identifier == NULL          || strcmp(identifier, currentoper->identifier) == 0) &&
              (precedence == -1            || precedence == currentoper->precedence) &&
              (arity == OPERATORARITY_NONE || 
                ((arity == currentoper->arity) &&
                    (arity == OPERATORARITY_UNARY && (UnaryOperatorAffix)arity_specific_value == currentoper->affix) ||
                    (arity == OPERATORARITY_BINARY && (BinaryOperatorAssociativity)arity_specific_value == currentoper->associativity)
                )
              )
          )
        {
            current = append_llnode(current, create_filledll_nocopy(currentoper, 0));
        }
    }
    return matches;
}
