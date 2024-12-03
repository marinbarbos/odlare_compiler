#include <mypas.h>
#include <parser.h>
#include <lexer.h>
#include <libgen.h>

FILE *src;

int main(int argc, char *argv[])
{
    // Verifica se foi informado um nome de arquivo para ser lido
    if (argc == 1)
    {
        fprintf(stderr, "%s: missing file name\n", basename(argv[0]));
    }
    else if (argc == 2)
    {
        // Tenta abrir o arquivo informado em modo leitura
        src = fopen(argv[1], "r");

        if (src == NULL)
        {
            // Caso não encontre o arquivo, imprime não encontrado e encerra a aplicação.
            fprintf(stderr, "%s: not found\n", argv[1]);
            exit(-3);
        }
    }
    else
    {
        // Imprime erro e encerra a aplicação caso passado mais de um parâmetro
        fprintf(stderr, "%s: too many paramters\n", argv[0]);
        exit(-4);
    }
    // src = stdin;
    lookahead = gettoken(src);
    
    // Inicia a função de compilação
    mypas();

    return 0;
}
