#include <ctype.h>
#include <stdio.h>
#include <lexer.h>

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

void skipSpaces(FILE *tape)
{
    int head;

    // Ignora espaços e conta a quantidade de linhas.
    while (isspace(head = getc(tape)))
        if (head == '\n')
            linenum++;

    ungetc(head, tape);
}

// 1. Apresente uma expressão regular para definir números octais. O número 0 é octal? Justifique sua resposta.
// OCT = 0[0-7]*
// 012345670
// 0012
int isOCT(FILE *tape)
{
    int i = 0;
    lexeme[i] = getc(tape);
    // Verifica se há uma sequência de dígitos prefixados por um 0 e seguidos de um ou mais dígitos não maiores do que 7.
    if (lexeme[i] == '0')
    {
        i = 1;
        while (isdigit(lexeme[i] = getc(tape)))
        {
            // Caso o caracter recebido seja maior do que 7 é necessário devolver lexeme encontrado e o prefixo 0.
            if (lexeme[i] > '7')
            {
                ungetc(lexeme[i], tape);
                ungetc('0', tape);
                i--;
                lexeme[i] = 0;
                return 0;
            }

            i++;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return OCT;
    }

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

// 5. Análogo ao Exercício 2, implemente uma função predicado, int isHEX(FILE *tape), para decidir se uma cadeia, oriunda de tape, é ou não um hexadecimal.
// HEX = 0[xX][0-9A-F]+
// 0xFF
// 0x112
int isHEX(FILE *tape)
{
    int i = 0;
    lexeme[i] = getc(tape);

// Verifica se há uma sequência de letras e/ou dígitos decimais prefixados por um 0x e se letra não maior que F.
    if (isdigit(lexeme[i]))
    {
        i = 1;
        if (lexeme[i] == '0' && (toupper(lexeme[i] = getc(tape)) == 'X'))
        {
            do
            {
                if (!(isalnum(lexeme[i] = getc(tape))) || lexeme[i] > 'F')
                {
                    ungetc(lexeme[i], tape);
                    lexeme[i] = 0;
                    return 0;
                }
            } while (isalnum(lexeme[i] = getc(tape)));

            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return HEX;
        }
    }
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

int isNUM(FILE *tape)
{
    int charCount;
    charCount = fscanf(tape, "%s", &lexval);
    if (charCount)
        return NUM;

    return 0;
}

/* FP = DEC.[0-9]* | .[0-9]+ */
int isFP(FILE *tape)
{
    int i;

    // Verifica se há uma sequência de dígitos decimais seguido de um único ponto e de 0 ou outros dígitos decimais.
    if (isDEC(tape))
    {
        i = strlen(lexeme);
        lexeme[i] = getc(tape);

        // Após encontrar um número decimal, caso não venha um ponto, significa que ele é um decimal não flutuante.
        if (lexeme[i] != '.')
        {
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return DEC;
        }

        i++;

        // Continua realizando a leitura de dígitos após o ponto, caso existam.
        while (isdigit(lexeme[i] = getc(tape)))
            i++;

        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return 1;
    }

    i = 0;
    lexeme[i] = getc(tape);

    // Verifica se há uma sequência de um ponto seguido de dígitos decimais.
    if (lexeme[i] == '.')
    {
        i++;
        // Caso não venha um número após o ponto, o segmento lido é devolvido a fita e o método abortado.
        if (!isdigit(lexeme[i] = getc(tape)))
        {
            ungetc(lexeme[i], tape);
            ungetc('.', tape);
            i--;
            lexeme[i] = 0;
            return 0;
        }

        // Continua realizando a leitura de dígitos após o ponto, caso existam.
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

int isFP2(FILE *tape)
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

int getToken(FILE *source)
{
    int token;
    skipSpaces(source);

    if ((token = isHEX(source)))
        return token;
    if ((token = isID(source)))
        return token;
    if ((token = isOCT(source)))
        return token;
    if ((token = isNUM(source)))
        return token;
    if ((token = isDEC(source)))
        return token;
    if ((token = isASGN(source)))
        return token;

    token = getc(source);
    return token;
}