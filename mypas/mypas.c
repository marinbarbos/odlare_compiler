#include <stdio.h>
#include <parser.h>
#include <lexer.h>

FILE *src;

extern int linenum;
int main()
{
    src = stdin;
    lookahead = gettoken(src);

    // Coração da aplicação
    mypas();

    printf("\n%i", lookahead);
    printf("\nNumero de linhas: %i", linenum);
    return 0;
}
