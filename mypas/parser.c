#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>

void program(void)
{
    match(PROGRAM);
    match(ID);
    match('(');
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
        idlist();
        match(':');
        type();
        match(';');
        if (lookahead == ';')
        {
            goto _idlist;
        }
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
        match(':');
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
        repstmt();
        break;
    default:;
    }
}

void ifstmt(void)
{
    if (lookahead == IF)
    match(IF);
    expr();
    stmt();
    if (lookahead == ELSE)
    {
        match(ELSE);
        stmt();
    }
}

void idstmt(void)
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
            exprlist();
        }
    }
}

void expr(void)
{
    switch(lookahead) {
        case '<':
            if()
            match('<');
            break;
        case '>':
            match('>');
            break;
        case '=':
            match('=');
            break;
        case IN:
            match(IN);
            break;

        default:
            break;
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