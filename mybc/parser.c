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
S → aSR | c
R → bS | <>
*/

// E -> [oplus] T R
// oplus = '+' || '-'
void E(void) { 
    if(lookahead == '+' || lookahead == '-') {
        match(lookahead);
    }
_T():
    T(); 
    // R -> {oplus T }
    // oplus = '+' || '-'
    while(lookahead == '+' || lookahead == '-') match(lookahead); goto _T();
}

// T -> F Q
void T(void) { 
_F():
    F(); 
    // Q -> {otimes F }
    // otimes = '*' || '/'
    while(lookahead == '*' || lookahead == '/') match(lookahead); goto _F();
    
}

// F -> (E) | DEC | ID | OCT | HEX
void F(void)
{
    switch(lookahead) {
        case '(':
            match('('); E(); match(')');
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

// R -> {oplus T }
// oplus = '+' || '-'
void R(void)
{
    while(lookahead == '+' || lookahead == '-') match(lookahead); T();
}


// Q -> {otimes F }
// otimes = '*' || '/'
void Q(void)
{
    while(lookahead == '*' || lookahead == '/') match(lookahead); F();
}

void match(int expected)
{
    if (lookahead == expected) {
        lookahead = gettoken(source);
    }
    else {
        fprintf(stderr, "token mismatch\n");
        exit(-3);
    }
}
