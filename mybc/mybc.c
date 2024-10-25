#include <stdio.h>
#include <parser.h>
#include <lexer.h>

FILE *source;

extern int linenum;
int main()
{
    source = stdin;
    lookahead = getToken(source);

    // Coração da aplicação
    mybc();

    printf("\n%i", lookahead);
    printf("\nNumero de linhas: %i", linenum);
    return 0;
}
