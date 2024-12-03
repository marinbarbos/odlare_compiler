#include <ctype.h>
#include <stdio.h>
#include <lexer.h>
#include <string.h>

char lexeme[MAXIDLEN + 1];

int linenum = 1;
double lexval;

/* Função que determina se o lexeme é uma atribuição (assignment)
   ASGN = := */
int isASGN(FILE *tape)
{
    // Lê o primeiro caracter da fita
    lexeme[0] = getc(tape);

    // Verifica se o primeiro caracter é ':'
    if (lexeme[0] == ':')
    {
        // Caso o primeiro caracter seja ':', verifica se o próximo é '='
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=')
        {
            /* Caso tenha passado nas validações anteriores,
            "limpa" o próximo caracter do lexeme e retorna que é um ASGN */
            lexeme[2] = 0;
            return ASGN;
        }

        /* Caso o segundo caracter da fita não seja '=',
        ele é devolvido à fita e o caracter do lexeme zerado.*/
        ungetc(lexeme[1], tape);
        lexeme[1] = 0;
    }

    /* Caso o primeiro caracter da fita não seja ':',
    ele é devolvido à fita e o caracter atual do lexeme zerado */
    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

/* Função que identifica se o lexeme é um identificador válido
   ID = [A-Za-z][A-Za-z0-9]* */
int isID(FILE *tape)
{
    int i = 0;

    // Lê o primeiro caractere da fita
    lexeme[i] = getc(tape);

    /* Verifica se o primeiro caractere é uma letra (prefixo necessário para um identificador) */
    if (isalpha(lexeme[i]))
    {
        i = 1;

        /* Continua lendo caracteres enquanto forem letras ou dígitos,
        armazenando-os no lexeme até atingir o limite máximo permitido */
        while (isalnum(lexeme[i] = getc(tape)))
        {
            if (i < MAXIDLEN)
                i++;
        }

        /* Devolve o último caractere não reconhecido como parte do identificador à fita */
        ungetc(lexeme[i], tape);

        // Finaliza o lexeme com '0'
        lexeme[i] = 0;

        /* Verifica se foi solicitado para abortar a aplicação com EXIT ou QUIT*/
        if (strcmp(lexeme, "exit") == 0)
            return EXIT;

        if (strcmp(lexeme, "quit") == 0)
            return QUIT;

        /* Verifica se o identificador é uma keyword, sendo assim, não é um identificador. */
        if (i = iskeyword(lexeme))
        {
            return i;
        }

        /* Retorna que o lexeme é um identificador válido */
        return ID;
    }

    /* Caso o primeiro caractere não seja uma letra, devolve-o à fita e zera o lexeme */
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/* Função que identifica se o lexeme é um operador relacional válido
   RELOP = < | <= | > | >= | = | <> */
int isRELOP(FILE *tape)
{
    // Lê o primeiro caractere da fita
    switch (lexeme[0] = getc(tape))
    {
    case '<':
        // Verifica se o próximo caractere é '=', formando '<='
        if ((lexeme[1] = getc(tape)) == '=')
        {
            return LEQ; // "menor ou igual"
        }
        // Verifica se o próximo caractere é '>', formando '<>'
        if (lexeme[1] == '>')
        {
            return NEQ; // "diferente"
        }
        // Caso contrário, devolve o caractere à fita, visto que temos um '<' simples
        ungetc(lexeme[1], tape);
        return LT; // "menor que"

    case '>':
        // Verifica se o próximo caractere é '=', formando '>='
        if ((lexeme[1] = getc(tape)) == '=')
        {
            return GEQ; // "maior ou igual"
        }
        // Caso contrário, devolve o caractere à fita, visto que temos um '>' simples
        ungetc(lexeme[1], tape);
        return GT; // "maior que"

    case '=':
        // Caso o primeiro caracter seja '=', este por si já corresponde ao operador de igualdade
        return EQ; // "igual"
    };

    /* Caso o primeiro caractere não seja parte de um operador, devolve-o à fita e zera o lexeme */
    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}


/* Função que determina se o lexeme é um número inteiro ou flutuante válido */
int isNUM(FILE *tape)
{
    // Lê o próximo caracter da fita para análise
    int aux = getc(tape);

    // Verifica se o caracter lido é um dígito numérico
    if (isdigit(aux))
    {
        // Devolve o caracter para a fita, pois será processado pelo fscanf
        ungetc(aux, tape);

        /* Lê um número de ponto flutuante da fita e armazena em lexval.
        O fscanf utilizado com a máscara "%lg" lê o número como float */
        fscanf(tape, "%lg", &lexval);

        /* Converte o número lido para uma string e o armazena em lexeme */
        sprintf(lexeme, "%lg", lexval);

        // Retorna que o lexeme é um número válido
        return NUM;
    }

// Função que ignora espaços vazios e comentários ao longo do código
void skip_spaces_comments(FILE *tape)
{
    int head;

_skip_spaces_comments:
    // Ignora os espaços
    while (isspace(head = getc(tape)))
    {
        // Ao encontrar uma quebra de linha, incrementa a variável de número de linhas
        if (head == '\n')
        {
            linenum++;
        }
    }

    // Verifica se o próximo caracter que não é espaço ou quebra de linha é '{'
    if (head == '{')
    {
        // Enquanto não encontrar um '}', não acabou o comentário.
        while (!((head = getc(tape)) == '}'))
        {
            // Caso encontre uma quebra de linha, incrementa a variável de número de linhas
            if (head == '\n')
            {
                linenum++;
            }

            // Caso encontre um fim de arquivo devolve o caracter à fita e aborta o laço.
            if (head == EOF)
            {
                ungetc(head, tape);
                break;
            }
        }

        goto _skip_spaces_comments;
    }

    /* Devolve o último caracter à fita, seja porque não era o início de um comentário, 
       seja porque encontrou um EOF */
    ungetc(head, tape);
}
    /* Caso o caracter lido não seja um dígito, devolve-o à fita
    e retorna 0 para indicar que não foi encontrado um número */
    ungetc(aux, tape);
    return 0;
}

int gettoken(FILE *src)
{
    int token;
    skip_spaces_comments(src);

    if ((token = isID(src)))
        return token;
    if ((token = isNUM(src)))
        return token;
    if ((token = isASGN(src)))
        return token;
    if ((token = isRELOP(src)))
        return token;
    if ((token = isDEC(src)))
        return token;

    token = getc(src);
    return token;
}
