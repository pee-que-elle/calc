#include <stdio.h>
#include "linkedlist.h"

#include "ast.h"
#include "lexer.h"
#include "parser.h"

int main(unsigned argc, char** argv)
{
    LinkedList_T *t = lex(argv[1]);
    LinkedList_T *current = t;
    
    if(t == NULL) return 1;

    printf("Lexer info: ");
    while(1)
    {
        if(current == NULL || current->value == NULL) break;
        LexerToken_T *t = (LexerToken_T*)current->value;
        printf("(%s '%s') ", toktype2str(t->type), tok2str(t)); 
        current = current->next;
    }

    putchar('\n');

    ASTNode_T *parsed = parse(t);
    
    if(parsed == NULL)
    {
        return 1;
    }

    printf("%s", ast2str(parsed));
    
    putchar('\n');

    ll_free(t);
}
