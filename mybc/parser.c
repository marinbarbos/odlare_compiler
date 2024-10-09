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
    /*0*/
    int oplus = 0;
    int signal = 0;
    int otimes = 0;
    /*0*/

    if(lookahead == '+' || lookahead == '-') {
        match(lookahead);
    }

_T:

_F:
    int lexema = lookahead;
    switch(lookahead) {
        case '(':
            match('('); 
            E(); 
            match(')');
            break;
        case DEC:
            /*1*/printf(" %s ", lexema);/*1*/
            match(DEC);
            break;
        case OCT:
            /*2*/printf(" %s ", lexema);/*2*/
            match(OCT);
            break;
        case HEX:
            /*3*/printf(" %s ", lexema);/*3*/
            match(HEX);
            break;
        default:
            /*4*/printf(" %s ", lexema);/*4*/
            match(ID);
    }
    if(otimes) {
        printf(" %c ", otimes);
        otimes = 0;
    }
    // Q -> {otimes F }printf(" %s ", lexema);
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
