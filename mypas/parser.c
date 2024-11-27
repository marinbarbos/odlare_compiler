#include <mypas.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>
#include <keywords.h>
#include <symtab.h>

int lookahead;
int lexlevel = 1;
int error_count = 0;

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

void block(void)
{
    vardef();
    sbprgdef();
    beginend();
    match('.');
}

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

void idlist(int localLexLevel)
{
    int error_stat = 0;
_idlist:
    error_stat = symtab_append(lexeme, localLexLevel);
    if (error_stat)
    {
        fprintf(stderr, "FALTAL ERROR: symbol already defined\n");
        error_count++;
    }
    match(ID);
    if (lookahead == ',')
    {
        match(',');
        goto _idlist;
    }
}

int relop()
{
    switch (lookahead)
    {
    case '>':
    case GEQ:
    case '<':
    case LEQ:
    case NEQ:
    case '=':
        return (lookahead);
    }

    return 0;
}

void beginend(void)
{
    match(BEGIN);
    stmtlist();
    match(END);
}

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

void stmt(void)
{
    fprintf(stderr, "ENTROU stmt: \n");

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

void idstmt(void)
{
    int id_position = symtab_lookup(lexeme, lexlevel);

    if (id_position < 0)
    {
        fprintf(stderr, "FATAL ERROR: symbol not defined\n");
        error_count++;
    }
    fprintf(stderr, "ID TABELA SYMBOL: %d\n", id_position);

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

void sbprgdef(void)
{
    int local_init_position, error_start;

    while (lookahead == PROCEDURE || lookahead == FUNCTION)
    {
        int isfunc = (lookahead == FUNCTION);
        match(lookahead);
        error_start = symtab_append(lexeme, lexlevel);

        if (error_start)
        {
            fprintf(stderr, "FATAL ERROR: already defined\n");
            error_count++;
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
        lexlevel++; // Incrementa nível léxico
        block();
        match(';');
        lexlevel--; // Decrementa nível léxico
    }
}

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

void whilestmt(void)
{
    match(WHILE);
    expr();
    match(DO);
    stmt();
}

void repeatstmt(void)
{
    match(REPEAT);
    stmtlist();
    match(UNTIL);
    expr();
}

void smpexpr(void)
{
    if (lookahead == '+' || lookahead == '-')
        match(lookahead);
_simpleexpr:
    term();
    if (lookahead == '+' || lookahead == '-' || lookahead == OR)
    {
        match(lookahead);
        goto _simpleexpr;
    }
}

void expr(void)
{
    smpexpr();

    if (relop())
    {
        match(lookahead);
        smpexpr();
    }
}

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

void factor(void)
{
    switch (lookahead)
    {
    // case UC:
    //     uc();
    //     break;
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
    fprintf(stderr, "%d | %d\n", lookahead, expected);
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
