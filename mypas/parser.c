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