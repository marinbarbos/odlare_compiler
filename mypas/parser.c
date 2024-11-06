#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>
#include <keywords.h>

int lookahead;

void program(void)
{
    match(PROGRAM);
    match(ID);
    match('(');
    idlist();
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
        match(VAR);
_idlist:
    idlist();
    match(':');
    type();
    match(';');
    if (lookahead == ID)
        goto _idlist;
}

void idlist(void)
{
_idlist:
    match(ID);
    if (lookahead == ',')
    {
        match(',');
        goto _idlist;
    }
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
    match(ID);
    if (lookahead == ASGN) {
        match(ASGN);
        expr();
    } else {
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
    while (lookahead == PROCEDURE || lookahead == FUNCTION)
    {
        int isfunc = (lookahead == FUNCTION);
        match(lookahead);
        match(ID);
        parmlist();
        if (isfunc)
        {
            match(':');
            type();
        }
        match(';');
        block();
        match(';');
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

        idlist();
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
    if (lookahead == ELSE) {
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

void expr(void)
{
    simpleexpr();
    if (lookahead == IN)
    {
        match(IN);
        simpleexpr();
    }
    else
    {
        match(lookahead);
        simpleexpr();
    }
}

void simpleexpr(void)
{
    if (lookahead == '+' || lookahead == '-')
        match(lookahead);
_simpleexpr:
    term();
    if (lookahead == '+' || lookahead == '-')
    {
        match(lookahead);
        goto _simpleexpr;
    }
    else if (lookahead == OR)
    {
        match(OR);
        goto _simpleexpr;
    }
}

void term(void)
{
_term:
    factor();
    switch (lookahead)
    {
    case '*':
        match('*');
        goto _term;
        break;
    case '/':
        match('/');
        goto _term;
        break;
    case DIV:
        match(DIV);
        goto _term;
        break;
    case MOD:
        match(MOD);
        goto _term;
        break;
    case AND:
        match(AND);
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

void mypas(void)
{
    while (lookahead == ';' || lookahead == '\n')
    {
        match(lookahead);
    }
    match(EOF);
}
