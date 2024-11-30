#include <ctype.h>
#include <stdio.h>
#include <lexer.h>
#include <string.h>

// Variáveis globais
char lexeme[MAXIDLEN + 1]; // Lexeme atual
int linenum = 1;           // Quantidade de linhas
double lexval;             // Valor léxico

/* Função que determina se o lexeme é uma atribuição (assignment) 
   ASGN = := */
int isASGN(FILE *tape)
{
    // Armazena o primeiro caracter da fita para ser analisado
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

/* Função que determina se o lexeme é um identificador (ID) ou comando de saída 
   ID = [A-Za-z][A-Za-z0-9]*
   Comandos de saída = QUIT | EXIT */
int isID(FILE *tape)
{
    int i = 0;

    // Armazena o primeiro caracter da fita para análise
    lexeme[i] = getc(tape);

    // Verifica se o primeiro caracter é uma letra (A-Z ou a-z)
    if (isalpha(lexeme[i]))
    {
        // Inicializa o índice para continuar a leitura
        i = 1;

        // Continua lendo caracteres enquanto forem letras ou dígitos
        while (isalnum(lexeme[i] = getc(tape)))
        {
            // Garante que o tamanho do identificador não ultrapasse o limite permitido
            if (i < MAXIDLEN)
                i++;
        }

        /* Quando um caracter não alfanumérico for encontrado, devolve-o para a fita
        e encerra a string do lexeme com '\0' */
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;

        // Verifica se o identificador lido corresponde à palavra-chave "exit"
        if (strcmp(lexeme, "exit") == 0)
            return EXIT;

        // Verifica se o identificador lido corresponde à palavra-chave "quit"
        if (strcmp(lexeme, "quit") == 0)
            return QUIT;

        // Caso seja apenas um identificador válido, retorna ID
        return ID;
    }

    /* Caso o primeiro caracter não seja uma letra, devolve-o à fita
    e zera o lexeme para indicar que não foi encontrado um identificador */
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/* Função que determina se o lexeme é um número decimal (DEC) 
   DEC = [1-9][0-9]* | 0 */
int isDEC(FILE *tape)
{
    int i = 0;

    // Armazena o primeiro caracter da fita para análise
    lexeme[i] = getc(tape);

    // Verifica se o primeiro caracter é um dígito numérico
    if (isdigit(lexeme[i]))
    {
        // Caso o dígito seja '0', é retornado imediatamente como um número decimal válido
        if (lexeme[i] == '0')
        {
            return DEC;
        }

        // Caso contrário, continua lendo a sequência de dígitos
        i++;
        while (isdigit(lexeme[i] = getc(tape)))
            i++;

        /* Quando um caracter não numérico for encontrado, devolve-o para a fita
        e encerra a string do lexeme com 0 */
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return DEC;
    }

    /* Caso o primeiro caracter não seja um dígito, devolve-o à fita
    e zera o lexeme para indicar que não foi encontrado um número decimal */
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

/* Função que determina se o lexeme é um número octal (OCT) */
/* OCT = 0[0-7]* */
int isOCT(FILE *tape)
{
    int i = 0;

    // Armazena o primeiro caracter da fita para análise
    lexeme[i] = getc(tape);

    // Verifica se o primeiro caracter é '0', indicando o início de um número octal
    if (lexeme[i] == '0')
    {
        // Inicializa o índice para continuar a leitura dos dígitos
        i = 1;

        // Continua lendo caracteres enquanto forem dígitos
        while (isdigit(lexeme[i] = getc(tape)))
        {
            /* Caso o dígito seja maior que '7', devolve o caracter lido e o prefixo '0'
            para a fita, limpa o lexeme e encerra a validação */
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

        /* Quando um caracter não numérico for encontrado, devolve-o para a fita
        e encerra a string do lexeme com '0' */
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;

        // Retorna que o lexeme é um número octal válido
        return OCT;
    }

    /* Caso o primeiro caracter não seja '0', devolve-o à fita
    e zera o lexeme para indicar que não foi encontrado um número octal */
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/* Função que determina se o lexeme é um número hexadecimal (HEX) */
/* HEX = 0[xX][0-9A-F]+ */
int isHEX(FILE *tape)
{
    int i = 0;

    // Armazena o primeiro caracter da fita para análise
    lexeme[i] = getc(tape);

    // Verifica se o primeiro caracter é um dígito numérico
    if (isdigit(lexeme[i]))
    {
        // Verifica se o prefixo é '0x' ou '0X'
        i = 1;
        if (lexeme[i] == '0' && (toupper(lexeme[i] = getc(tape)) == 'X'))
        {
            // Lê a sequência de caracteres hexadecimal
            do
            {
                /* Caso o caracter lido não seja alfanumérico ou seja maior que 'F',
                devolve o caracter para a fita, zera o lexeme e retorna 0 */
                if (!(isalnum(lexeme[i] = getc(tape))) || lexeme[i] > 'F')
                {
                    ungetc(lexeme[i], tape);
                    lexeme[i] = 0;
                    return 0;
                }
            } while (isalnum(lexeme[i] = getc(tape)));

            /* Quando um caracter não hexadecimal for encontrado, devolve-o para a fita
            e encerra a string do lexeme com '\0' */
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;

            // Retorna que o lexeme é um número hexadecimal válido
            return HEX;
        }
    }

    /* Caso a sequência não corresponda a um hexadecimal,
    devolve o primeiro caracter à fita e zera o lexeme */
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
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

    /* Caso o caracter lido não seja um dígito, devolve-o à fita
    e retorna 0 para indicar que não foi encontrado um número */
    ungetc(aux, tape);
    return 0;
}

/* Função que determina se a sequência na fita é um número de ponto flutuante */
int isFP(FILE *tape)
{
    float value;

    /* Usa fscanf para tentar ler um número de ponto flutuante da fita.
    Caso consiga ler com sucesso, retorna o valor correspondente (diferente de zero). 
    Se não conseguir, retorna 0. */
    return fscanf(tape, "%f", &value);
}

/* Função que determina se o lexeme contém uma notação exponencial (EE)
   EE = [eE]['+''-']?[0-9]+ */
int isEE(FILE *tape)
{
    int i = strlen(lexeme); // Inicia o índice no final da string atual de lexeme
    int e, sign;           // Variáveis para armazenar os caracteres de exponencial (e|E) e sinal (+|-)

    // Lê o próximo caracter da fita e o armazena no lexeme
    lexeme[i] = getc(tape);

    // Verifica se o caracter é 'e' ou 'E'
    if (toupper(lexeme[i]) == 'E')
    {
        e = lexeme[i]; // Armazena o prefixo exponencial
        i++;
        lexeme[i] = getc(tape);

        // Verifica se o próximo caracter é um sinal '+' ou '-'
        if (lexeme[i] == '+' || lexeme[i] == '-')
        {
            sign = lexeme[i]; // Armazena o sinal
            i++;
            lexeme[i] = getc(tape);

            /* Caso o caracter após o sinal não seja um dígito, devolve o caracter,
            o sinal e o prefixo 'e|E' à fita, limpa o lexeme e retorna 0 */
            if (!isdigit(lexeme[i]))
            {
                ungetc(lexeme[i], tape);
                ungetc(sign, tape);
                ungetc(e, tape);
                i -= 2;
                lexeme[i] = 0;
                return 0;
            }

            // Continua lendo os dígitos após o sinal
            i++;
            while (isdigit(lexeme[i] = getc(tape)))
                i++;

            /* Quando um caracter não numérico for encontrado, devolve-o à fita
            e encerra a string do lexeme */
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;

            // Retorna que a sequência é uma notação exponencial válida
            return 1;
        }

        /* Caso o próximo caracter após 'e|E' não seja nem sinal nem dígito,
        devolve o caracter e o prefixo 'e|E' à fita e retorna 0 */
        if (!isdigit(lexeme[i]))
        {
            ungetc(lexeme[i], tape);
            ungetc(e, tape);
            i--;
            lexeme[i] = 0;
            return 0;
        }

        // Continua lendo os dígitos após 'e|E' se não houver um sinal
        i++;
        while (isdigit(lexeme[i] = getc(tape)))
            i++;

        // Devolve o último caracter lido à fita e encerra o lexeme
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;

        // Retorna que a sequência é uma notação exponencial válida
        return 1;
    }

    /* Caso o caracter inicial não seja 'e' ou 'E', devolve-o à fita
    e zera o lexeme para indicar que não foi encontrado uma notação exponencial */
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}


/* Função que determina se o lexeme é um número flutuante (FLT) */
/* FLT = FP EE? | DEC EE */
int isFLT(FILE *tape)
{
    int fp;

    // Verifica se a sequência na fita é um número de ponto flutuante (FP)
    if ((fp = isFP(tape)) == DEC)
    {
        // Se for um número decimal, verifica se há uma notação exponencial (EE)
        if (!isEE(tape))
            return DEC; // Retorna DEC se não houver exponencial

        // Se houver exponencial, retorna NUM para indicar um número flutuante
        return NUM;
    }
    else if (fp)
    {
        // Caso a sequência seja um número de ponto flutuante (FP), verifica a notação exponencial (EE)
        isEE(tape);

        // Retorna NUM indicando que a sequência é um número flutuante
        return NUM;
    }

    // Caso não corresponda a nenhum formato de número flutuante, retorna 0
    return 0;
}


/* Função que determina o próximo token a ser lido da fonte */
int getToken(FILE *source)
{
    int token;

    // Ignora espaços em branco antes de processar o próximo token
    skipSpaces(source);

    // if ((token = isHEX(source)))
    //     return token;

    // Verifica se o próximo token é um identificador e retorna o token correspondente
    if ((token = isID(source)))
        return token;

    // if ((token = isOCT(source)))
    //     return token;

    // Verifica se o próximo token é um número e retorna o token correspondente
    if ((token = isNUM(source)))
        return token;

    // if ((token = isDEC(source)))
    //     return token;

    // Verifica se o próximo token é uma atribuição e retorna o token correspondente
    if ((token = isASGN(source)))
        return token;

    // Se nenhum dos casos anteriores for atendido, lê um único caractere da fonte e o retorna como token
    token = getc(source);
    return token;
}
