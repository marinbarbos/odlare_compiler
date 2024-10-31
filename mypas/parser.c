#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>
#include <keywords.h>

void program(void)
{
    match(PROGRAM);
    match(ID);
    match('(');
    idList();
    match(')');
    match(';');
    block();
    match('.');
}

void block(void)
{
    vardef();
    sbprgdef();
    beginEnd();
    match('.');
}

void vardef(void)
{
    if (lookahead == VAR)
        match(VAR);
_idlist:
    idList();
    match(':');
    type();
    match(';');
    if (lookahead == ID)
        goto _idlist;
}

void idList(void)
{
_idlist:
    match(ID);
    if (lookahead == ',')
    {
        match(',');
        goto _idlist;
    }
}

void beginEnd(void)
{
    match(BEGIN);
    stmtList();
    match(END);
}

void stmtList(void)
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
        idStmt();
        break;
    case BEGIN:
        beginEnd();
        break;
    case IF:
        ifStmt();
        break;
    case WHILE:
        whileStmt();
        break;
    case REPEAT:
        repeatStmt();
        break;
    default:
        break;
    }
}

void idStmt(void)
{
    if (lookahead == ID)
    {
        match(ID);
        if (lookahead == ASGN)
        {
            match(ASGN);
            expr();
        }
        else
        {
            exprList();
        }
    }
}

void exprList(void)
{
    if (lookahead == '(')
    {
        match('(');
    _exprList:
        expr();
        if (lookahead == ',')
        {
            match(',');
            goto _exprList;
        }
        match(')');
    }
}

void sbrgdef(void)
{
    while (lookahead == PROCEDURE || lookahead == FUNCTION)
    {
        int isFunc = (lookahead == FUNCTION);
        match(lookahead);
        match(ID);
        parmList();
        if (isFunc)
        {
            match(':');
            type();
        }
        match(';');
        block();
        match(';');
    }
}

void parmList(void)
{
    if (lookahead == '(')
    {
        match('(');
    _parmList:
        if (lookahead == VAR)
        {
            match(VAR);
        }

        idList();
        match(':');
        type();
        if (lookahead == ';')
        {
            match(';');
            goto _parmList;
        }
        match(')');
    }
}

void ifStmt(void)
{
    if (lookahead == IF)
    {
        match(IF);
        expr();
        match(THEN);
        if (lookahead == ELSE)
        {
            match(ELSE);
            stmt();
        }
    }
}

void whileStmt(void)
{
    if (lookahead == WHILE)
    {
        match(WHILE);
        expr();
        match(DO);
        stmt();
    }
}

void repeatStmt(void)
{
    if (lookahead == REPEAT)
    {
        match(REPEAT);
        stmtList();
        match(UNTIL);
        expr();
    }
}

void expr(void)
{
    simpleExpr();
    if (lookahead == IN)
    {
        match(IN);
        simpleExpr();
    }
    else
    {
        match(lookahead);
        simpleExpr();
    }
}

void simpleExpr(void)
{
    if (lookahead == '+' || lookahead == '-')
        match(lookahead);
_simpleExpr:
    term();
    if (lookahead == '+' || lookahead == '-')
    {
        match(lookahead);
        goto _simpleExpr;
    }
    else if (lookahead == OR)
    {
        match(OR);
        goto _simpleExpr;
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
    case UC:
        uc();
        break;
    case VAR:
        vardef();
        break;
    case FUNCTION:
        sbrgdef();
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
        exprList();
        match(']');
        break;
    default:
        break;
    }
}

void match(int expected)
{
    if (lookahead == expected)
    {
        lookahead = getToken(source);
    }
    else
    {
        fprintf(stderr, "token mismatch\n");
        exit(-3);
    }
}