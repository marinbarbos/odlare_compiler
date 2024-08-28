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
    int oplus = 0;
    int signal = 0;
    int otimes = 0;

    if(lookahead == '+' || lookahead == '-') {
        signal = lookahead;
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
            printf(" dec ");
            match(DEC);
            break;
        case OCT:
            printf(" oct ");
            match(OCT);
            break;
        case HEX:
            printf(" hex ");
            match(HEX);
            break;
        default:
            printf(" id ");
            match(ID);
    }
    if(otimes) {
        printf(" %c ", otimes);
        otimes = 0;
    }
    // Q -> {otimes F }
    // otimes = '*' || '/'
    if (lookahead == '*' || lookahead == '/') { 
        otimes = lookahead;
        match(lookahead); 
        goto _F; 
    }
    if(signal) {
        printf(" %c ", signal);
        signal = 0;
    }
    if(oplus) {
        printf(" %c ", oplus);
        oplus = 0;
    }
    // R -> {oplus T }
    // oplus = '+' || '-'
    if (lookahead == '+' || lookahead == '-') { 
        oplus = lookahead;
        match(lookahead); 
        goto _T; 
    }
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
