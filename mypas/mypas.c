#include <stdio.h>
#include <parser.h>
#include <lexer.h>

FILE *src;

extern int linenum;
int main()
{
    src = stdin;
    lookahead = gettoken(src);

    if (argc == 1)
    {
        fprintf(stderr, "%s: missing file name\n", basename(argv[0]));
    }
    else if (argc == 2)
    {
        src = fopen(argv[1], "r");
        if (src == NULL)
        {
            fprintf(stderr, "Not found %s\n", argv[1]);
            exit(-1);
        }
    }
    else
    {
        fprintf(stderr, "%s: too many parameters\n", argv[0]);
        exit(-1);
    }

    // Coração da aplicação
    mypas();

    printf("\n%i", lookahead);
    printf("\nNumero de linhas: %i", linenum);
    return 0;
}
