#include <stdio.h>

#include "ast.h"
#include "lexer.h"


int main(unsigned argc, char** argv)
{
    LexerToken *t = lex(argv[1]);

    if(t == NULL) return 1;

    for(int i = 0; t[i].type != METATOKEN_TERMINATE; ++i)
    {
        printf("(%s '%s') ", toktype2str(t[i].type), tok2str(&t[i]));
    }
    putchar('\n');
}
