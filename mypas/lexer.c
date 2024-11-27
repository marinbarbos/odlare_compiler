#include <ctype.h>
#include <stdio.h>
#include <lexer.h>
#include <string.h>

char lexeme[MAXIDLEN + 1];

int linenum = 1;
double lexval;

// ASGN = :=
// Ex: :=
int isASGN(FILE *tape)
{
    lexeme[0] = getc(tape);
    if (lexeme[0] == ':')
    {
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=')
        {
            lexeme[2] = 0;
            return ASGN;
        }
        ungetc(lexeme[1], tape);
    }

    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

// ID = [A-Za-z][A-Za-z0-9]*
// Ex: abacaXI123
int isID(FILE *tape)
{
    int i = 0;
    lexeme[i] = getc(tape);

    // Verifica se há uma sequência de identificador prefixada por uma letra e seguida de um ou mais letras ou dígitos.
    if (isalpha(lexeme[i]))
    {
        i = 1;
        while (isalnum(lexeme[i] = getc(tape)))
        {
            if (i < MAXIDLEN)
                i++;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        fprintf(stderr, "lexeme: %s\n", lexeme);

        if (strcmp(lexeme, "exit") == 0)
            return EXIT;

        if (strcmp(lexeme, "quit") == 0)
            return QUIT;

        if (i = iskeyword(lexeme))
        {
            fprintf(stderr, "keyword: %d\n", i);

            return i;
        }
        return ID;
    }

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

// DEC = [1-9][0-9]* | 0
// 1234567890
// 0012 = 0 0 12
int isDEC(FILE *tape)
{
    int i = 0;
    lexeme[i] = getc(tape);

    // Verifica se há uma sequência de decimal.
    if (isdigit(lexeme[i]))
    {
        // Caso o primeiro dígito seja 0, este por si só é o número decimal encontrado.
        if (lexeme[i] == '0')
        {
            return DEC;
        }

        // Continua realizando a leitura de dígitos caso existam.
        i++;
        while (isdigit(lexeme[i] = getc(tape)))
            i++;

        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return DEC;
    }

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

int isRELOP(FILE *tape)
{
    switch (lexeme[0] = getc(tape))
    {
    case '<':
        if ((lexeme[1] = getc(tape)) == '=')
        {
            return LEQ;
        }
        if (lexeme[1] == '>')
        {
            return NEQ;
        }
        ungetc(lexeme[1], tape);
        return LT;
    case '>':
        if ((lexeme[1] = getc(tape)) == '=')
        {
            return GEQ;
        }
        ungetc(lexeme[1], tape);
        return GT;
    case '=':
        ungetc(lexeme[1], tape);
        return EQ;
        break;
    };

    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

void skipspaces(FILE *tape)
{
    int head;

    // Ignora espaços e conta a quantidade de linhas.
_skipspaces:
    while (isspace(head = getc(tape)))
    {
        if (head == '\n')
        {
            linenum++;
        }
        fprintf(stderr, "head: %c\n", head);
    }
    // skip comments {}
    if (head == '{')
    {
        while (!((head = getc(tape)) == '}'))
        {
            if (head == '\n')
            {
                linenum++;
            }
            if (head == EOF)
            {
                ungetc(head, tape);
                break;
            }
        }

        goto _skipspaces;
    }

    ungetc(head, tape);
}

int isNUM(FILE *tape)
{
    int aux = getc(tape);

    if (isdigit(aux))
    {
        ungetc(aux, tape);
        fscanf(tape, "%lg", &lexval);
        sprintf(lexeme, "%lg", lexval);
        return NUM;
    }

    ungetc(aux, tape);
    return 0;
}

int isFP(FILE *tape)
{
    float value;
    return fscanf(tape, "%f", &value);
}

/* EE = [eE]['+''-']?[0-9]+ */
int isEE(FILE *tape)
{
    int i = strlen(lexeme);
    int e, sign; // Variáveis que recebem os caracteres de exponencial e sinal, respectivamente.
    lexeme[i] = getc(tape);

    // Verifica se há uma sequência exponencial prefixada por e|E e seguida de sinal e/ou dígitos.
    if (toupper(lexeme[i]) == 'E')
    {
        e = lexeme[i];
        i++;
        lexeme[i] = getc(tape);

        if (lexeme[i] == '+' || lexeme[i] == '-')
        {
            sign = lexeme[i];
            i++;
            lexeme[i] = getc(tape);

            // Após encontrar um sinal, caso o próximo caracter não for um dígito, é necessário devolver a fita o caracter lido, o sinal e o prefixo E.
            if (!isdigit(lexeme[i]))
            {
                ungetc(lexeme[i], tape);
                ungetc(sign, tape);
                ungetc(e, tape);
                i -= 2;
                lexeme[i] = 0;
                return 0;
            }

            // Continua realizando a leitura de dígitos após o sinal, caso existam.
            i++;
            while (isdigit(lexeme[i] = getc(tape)))
                i++;

            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return 1;
        }

        // Após receber e|E, caso o próximo caracter não seja nem sinal nem dígito, é necessário devolver a fita o caracter lido e o prefixo e|E.
        if (!isdigit(lexeme[i]))
        {
            ungetc(lexeme[i], tape);
            ungetc(e, tape);
            i--;
            lexeme[i] = 0;
            return 0;
        }

        // Continua realizando a leitura de dígitos após o e|E, caso existam.
        i++;
        while (isdigit(lexeme[i] = getc(tape)))
            i++;

        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return 1;
    }

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/* FLT = FP EE? | DEC EE */
int isFLT(FILE *tape)
{
    int fp;

    if ((fp = isFP(tape)) == DEC)
    {
        if (!isEE(tape))
            return DEC;
        return NUM;
    }
    else if (fp)
    {
        isEE(tape);
        return NUM;
    }
    return 0;
}

int gettoken(FILE *src)
{
    int token;
    skipspaces(src);

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
