#include <mypas.h>
#include <parser.h>
#include <lexer.h>
#include <libgen.h>

FILE *src;

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
            fprintf(stderr, "%s: not found\n", argv[1]);
            exit(-3);
        }
    }
    else
    {
        fprintf(stderr, "%s: too many paramters\n", argv[0]);
        exit(-4);
    }
    // src = stdin;
    lookahead = gettoken(src);
    
    // Coração da aplicação
    mypas();

    return 0;
}
