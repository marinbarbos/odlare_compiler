#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include <lexer.h>

FILE *src;

extern int linenum;
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "%s: missing file name\n", basename(argv[0]));
    }
    else if (argc == 2)
    {
        src = fopen(argv[1], "r");

        if (src == NULL)
        {
            fprintf("%s: not found\n", argv[1]);
            exit(-3);
        }
    }
    else
    {
        fprintf(stderr, "%s: too many paramters\n", argv[0]);
        exit(-4);
    }
    src = stdin;
    lookahead = gettoken(src);

    // Coração da aplicação
    mypas();

    printf("\n%i", lookahead);
    printf("\nNumero de linhas: %i", linenum);
    return 0;
}
