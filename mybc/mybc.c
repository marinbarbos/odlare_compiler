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

    return 0;
}
