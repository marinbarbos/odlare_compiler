#include <stdio.h>
#include <parser.h>
#include <lexer.h>

FILE *source;

extern int linenum;
int main()
{
    source = stdin;
    lookahead = gettoken(source);
    E();
    printf("\n%i", lookahead);
    printf("\nNumero de linhas: %i", linenum);
    return 0;
}