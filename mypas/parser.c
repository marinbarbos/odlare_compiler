#include <mypas.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>
#include <keywords.h>
#include <symtab.h>

int lookahead;
int lexlevel = 1;
int error_count = 0;

// Programa contendo um ID, com parâmetros e um bloco
void program(void)
{
    match(PROGRAM);
    match(ID);
    match('(');
    idlist(lexlevel + 1);
    match(')');
    match(';');
    block();
    match('.');
}

// Bloco contendo declaração de variável, subprograma e bloco BEGINEND
void block(void)
{
    vardef();
    sbprgdef();
    beginend();
}

// Declara variaveis com IDLIST:TIPO;
void vardef(void)
{
    if (lookahead == VAR)
    {
        match(VAR);
    _idlist:
        idlist(lexlevel + 1);
        match(':');
        type();
        match(';');
        if (lookahead == ID)
            goto _idlist;
    }
}

// Verifica IDs na tabela de simbolos, retornando erro se estiverem repetidos
void idlist(int localLexLevel)
{
    /*0*/
    int error_stat = 0;
    /*0*/
_idlist:
    error_stat = symtab_append(lexeme, localLexLevel);
    /*1*/
    if (error_stat)
    {
        fprintf(stderr, "FATAL ERROR: symbol already defined\n");
        error_count++;
    }
    /*1*/
    match(ID);
    if (lookahead == ',')
    {
        match(',');
        goto _idlist;
    }
}

// Verifica se o token atual eh um operador lógico (< | <= | > | >= | == | <> | IN)
int relop()
{
    switch (lookahead)
    {
    case GT:
    case GEQ:
    case LT:
    case LEQ:
    case NEQ:
    case EQ:
    case IN:
        return (lookahead);
    }

    return 0;
}

// Estrutura de um bloco BEGIN com uma lista de statements e no final um END
void beginend(void)
{
    match(BEGIN);
    stmtlist();
    match(END);
}

// Lista de statements separados por ;
void stmtlist(void)
{
_stmtlist:
    stmt();
    if (lookahead == ';')
    {
        match(';');
        goto _stmtlist;
    }
}

// Estrutura de um statement que pode ser um ID, BEGINEND, IF, WHILE ou REPEAT
void stmt(void)
{
    switch (lookahead)
    {
    case ID:
        idstmt();
        break;
    case BEGIN:
        beginend();
        break;
    case IF:
        ifstmt();
        break;
    case WHILE:
        whilestmt();
        break;
    case REPEAT:
        repeatstmt();
        break;
    default:
        break;
    }
}

// Verifica declaração de variável
void idstmt(void)
{
    /*0*/
    int id_position = symtab_lookup(lexeme, lexlevel);
    /*0*/

    /*1*/
    if (id_position < 0)
    {
        fprintf(stderr, "FATAL ERROR: symbol not defined\n");
        error_count++;
    }
    /*1*/

    match(ID);
    if (lookahead == ASGN)
    {
        match(ASGN);
        expr();
    }
    else
    {
        exprlist();
    }
}

// lista de expressões separados por ","
void exprlist(void)
{
    if (lookahead == '(')
    {
        match('(');
    _exprlist:
        expr();
        if (lookahead == ',')
        {
            match(',');
            goto _exprlist;
        }
        match(')');
    }
}

// Um subprograma pode ser uma PROCEDURE ou uma FUNCTION, seguida de um ID, uma lista de parâmetros, separados por ";" e um bloco
void sbprgdef(void)
{
    int local_init_position, error_start;

    while (lookahead == PROCEDURE || lookahead == FUNCTION)
    {
        int isfunc = (lookahead == FUNCTION);
        match(lookahead);
        /*0*/
        error_start = symtab_append(lexeme, lexlevel);
        /*0*/

        if (error_start)
        {
            /*1*/
            fprintf(stderr, "FATAL ERROR: already defined\n");
            error_count++;
            /*1*/
        }

        match(ID);
        local_init_position = symtab_next_entry;
        parmlist(); // lexlevel + 1

        if (isfunc)
        {
            match(':');
            type();
        }

        match(';');
        /*2*/
        lexlevel++; // Incrementa nível léxico
        /*2*/
        block();
        match(';');
        /*3*/
        lexlevel--; // Decrementa nível léxico
        /*3*/
    }
}

// Lista de parâmetros dentro de parênteses, podendo ter declaração de variável, separado por ";"
void parmlist(void)
{
    if (lookahead == '(')
    {
        match('(');
    _parmlist:
        if (lookahead == VAR)
        {
            match(VAR);
        }

        idlist(lexlevel + 1);
        match(':');
        type();
        if (lookahead == ';')
        {
            match(';');
            goto _parmlist;
        }
        match(')');
    }
}

// Estrutura de decisão que contém uma condição (expressão), THEN um statement, ou um ELSE e um statement
void ifstmt(void)
{
    match(IF);
    expr();
    match(THEN);
    stmt();

    if (lookahead == ELSE)
    {
        match(ELSE);
        stmt();
    }
}

// Estrutura de repetição que contém uma condição (expressão) e um statement
void whilestmt(void)
{
    match(WHILE);
    expr();
    match(DO);
    stmt();
}

// Estrutura de repetição que contém uma lista de statements e um UNTIL com uma condição (expressão)
void repeatstmt(void)
{
    match(REPEAT);
    stmtlist();
    match(UNTIL);
    expr();
}

// Expressão simples é uma expressão (+ | -) termo, seguido de (+ | - | OR) outro termo
void smpexpr(void)
{
    if (lookahead == '+' || lookahead == '-')
        match(lookahead);
_simpleexpr:
    term();
    if (lookahead == '+' || lookahead == '-' || lookahead == OR)
    {
        match(lookahead);
        // goto _simpleexpr;
        term();
    }
}

// Expressão é uma expressão simples, seguido de relop e outra expressão simples
void expr(void)
{
    smpexpr();

    if (relop())
    {
        match(lookahead);
        smpexpr();
    }
}

// O termo é um fator, seguido de alguma operação (* | / | DIV | MOD | AND) e outro fator
void term(void)
{
_term:
    factor();
    switch (lookahead)
    {
    case '*':
    case '/':
    case DIV:
    case MOD:
    case AND:
        match(lookahead);
        goto _term;
        break;
    default:
        break;
    }
}

// Fator pode ser um NUM, um ID, uma VAR, uma FUNCTION, uma expressão, ou seguido de NOT
void factor(void)
{
    switch (lookahead)
    {
    case NUM:
        match(NUM);
        break;
    case ID:
        idstmt();
        break;
    case VAR:
        vardef();
        break;
    case FUNCTION:
        sbprgdef();
        break;
    case '(':
        match('(');
        expr();
        match(')');
        break;
    case NOT:
        factor();
        break;
    case '[':
        match('[');
        exprlist();
        match(']');
        break;
    default:
        break;
    }
}

// Os tipos possíveis são INTEGER, REAL, DOUBLE, BOOLEAN
void type(void)
{
    switch (lookahead)
    {
    case INTEGER:
    case REAL:
    case DOUBLE:
        match(lookahead);
        break;
    default:
        match(BOOLEAN);
        break;
    }
}

void match(int expected)
{
    if (lookahead == expected)
    {
        lookahead = gettoken(src);
    }
    else
    {
        fprintf(stderr, "syntax error at line %d\n", linenum);
        exit(-3);
    }
}

void cmd(void)
{
    // double result;
    switch (lookahead)
    {
    case ';':
    case '\n':
    case EOF:
        /* do nothing */
        break;

    case QUIT:
        exit(0);
    case EXIT:
        exit(0);

    default:
        program();
    }
}

void mypas(void)
{
    cmd();
    while (lookahead == ';' || lookahead == '\n')
    {
        match(lookahead);
        cmd();
    }
    match(EOF);
}
