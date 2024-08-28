#include <lexer.h>
#include <parser.h>
int lookahead;

/*
LL(1) grammar:

E -> [oplus] T R
oplus = '+' || '-'
*/

// E -> [oplus] T R
// oplus = '+' || '-'
void E(void) { 
    if(lookahead == '+' || lookahead == '-') {
        match(lookahead);
    }

_T:

_F:

    switch(lookahead) {
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
    // Q -> {otimes F }
    // otimes = '*' || '/'
    if (lookahead == '*' || lookahead == '/') { match(lookahead); goto _F; }

    // R -> {oplus T }
    // oplus = '+' || '-'
    if (lookahead == '+' || lookahead == '-') { match(lookahead); goto _T; }
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
