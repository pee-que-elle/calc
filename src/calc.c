#include <stdio.h>
#include <stdlib.h>
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

        char *tokstr = tok2str(t);

        printf("(%s '%s') ", toktype2str(t->type), tokstr);

        free(tokstr);

        current = current->next;
    }

    putchar('\n');

    ASTNode_T *parsed = parse(t);
    
    if(parsed == NULL)
    {
        return 1;
    }
    
    char *aststr = ast2str(parsed);
    printf("%s", aststr);
    
    putchar('\n');

    free(aststr);
    ast_free(parsed);
    ll_free(t, freetoken);
}
