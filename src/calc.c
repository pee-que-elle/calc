#include <stdio.h>
#include "linkedlist.h"

#include "ast.h"
#include "lexer.h"


int main(unsigned argc, char** argv)
{
    LinkedList_T *t = lex(argv[1]);
    LinkedList_T *current = t;
    while(1)
    {
        if(current->value == NULL) break;
        LexerToken_T *t = (LexerToken_T*)current->value;
        printf("(%s '%s') ", toktype2str(t->type), tok2str(t)); 
        current = current->next;
    }


    putchar('\n');
}
