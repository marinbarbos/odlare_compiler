#include <lexer.h>
#include <parser.h>
int lookahead;

/*
LL(1) grammar:

E -> T R
T -> F Q
F -> (E) | DEC | ID
R -> +T R | -T R | <>
Q -> *F Q | /F Q | <>
*/

// E -> T R
void E(void)
{
    if (lookahead == '+' || lookahead == '-')
    {
        match(lookahead);
    }
    T();
    R();
}

// T -> F Q
void T(void)
{
    F();
    Q();
}

// F -> (E) | DEC | ID
void F(void)
{
    switch (lookahead)
    {
    case '(':
        match('(');
        E();
        match(')');
        break;
    case DEC:
        match(DEC);
        break;
    case OCT:
        match(OCT);
        break;
    case HEX:
        match(HEX);
        break;
    default:
        match(ID);
    }
}

// R -> +T R | -T R | <>
void R(void)
{
    switch (lookahead)
    {
    case '+':
        match('+');
        T();
        R();
        break;
    case '-':
        match('-');
        T();
        R();
        break;
    default:;
    }
}

// Q -> *F Q | /F Q | <>
void Q(void)
{
    switch (lookahead)
    {
    case '*':
        match('*');
        F();
        Q();
        break;
    case '/':
        match('/');
        F();
        Q();
        break;
    default:;
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
