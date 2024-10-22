// 24.10/v7.1
#include <lexer.h>
#include <parser.h>
int lookahead;

#include <string.h>
#define SYMTABSIZE 0x100 // 256
double acc;
char symtab[SYMTABSIZE][MAXIDLEN+1];
double vmeme[SYMTABSIZE];
int symtab_next_entry = 0;

#define STACKSIZE 1024
double pilha[STACKSIZE];
int sp = -1;

void push(double value)
{
	sp++;
	pilha[sp] = value;
}

double pop(void)
{
	double value = pilha[sp];
	sp--;
	return value;
}

int lookup(char* varname) {
    int i;
	for (i = 0; i < symtab_next_entry; i++)
	{
		if (strcmp(symtab[i],varname) == 0) break;
	}
    if( i == symtab_next_entry) {
        strcpy(symtab[i], varname);
        symtab_next_entry++;
    }
    return i;
}

double recall(char* varname) {
    int i;
	for (i = 0; i < symtab_next_entry; i++) {
		if (strcmp(symtab[i], varname) == 0) return vmeme[i];
	}
	strcpy(symtab[symtab_next_entry], varname);
	symtab_next_entry++;
	return 0.0;
}

void store(char* varname, double value) {
    int i = lookup(varname);
    vmeme[i] = value;
}

// E -> [oplus] T R
// oplus = '+' || '-'
void E(void) { 
    /*0*/
    int oplus = 0;
    int signal = 0;
    int otimes = 0;
    /*0*/

    if(lookahead == '+' || lookahead == '-') {
        !(lookahead == '-' || signal == lookahead);
        match(lookahead);
    }

_T:
    T();

    /*1*/
    if(signal) {
        acc = -acc;
        printf ("\tneg acc\n");
        signal = 0;
    }
    /*1*/
    
    /*2*/
    if(oplus) {
        switch(oplus) {
            case '+': {
                pilha[sp] = pilha[sp] + acc;
                printf ("\tadd acc, stack[sp]\n", oplus);
                break;
            }
            case '-': {
                pilha[sp] = pilha[sp] - acc;
                printf ("\tsub stack[sp], acc\n", oplus);
                break;
            }
        }
        printf("\tpop acc\n");
        acc = pop();
        oplus = 0;
    }
    /*2*/

    if(lookahead == '+' || lookahead == '-') {
        /*3*/
        oplus = lookahead;
        printf("\tpush acc\n");
        push(acc);
        /*3*/
        match(lookahead);
        goto _T;
    }
}

/* T -> F {*F} */
void T(void)
{
    int otimes = 0;
_F:
    F();

    /*4*/
    switch(otimes) {
        case '*': {
            pilha[sp] = pilha[sp] * acc;
            printf("\tmul stack[sp], acc\n", otimes);
            break;
        }
        case '/': {
            pilha[sp] = pilha[sp] / acc;
            printf("\tdiv stack[sp], acc\n", otimes);
            break;
        }
    }
    acc = pop();
    otimes = 0;
    /*4*/

    if (lookahead == '*' || lookahead == '/') {
        /*5*/
        otimes = lookahead;
        printf("\tpush acc\n");
        push(acc);
        /*5*/
        match(lookahead);
        goto _F;
    }
}


/* F -> ID | DEC | OCT | HEX | FLT | ( E ) */
void F(void)
{
    /*0*/char varname[MAXIDLEN+1]; /*0*/
    switch(lookahead) {
    case '(':
        match('('); 
        E(); 
        match(')');
        break;
    case OCT:
        acc = strtol(lexeme, NULL, 8);
        /*3*/printf("\toctmov %s, acc\n", lexeme);/*3*/
        match(OCT);
        break;
    case DEC:
        acc = strtol(lexeme, NULL, 10);
        /*1*/printf("\tdecmov %s, acc\n", lexeme);/*1*/
        match(DEC);
        break;
    case HEX:
        acc = strtol(lexeme, NULL, 16);
        /*3*/printf("\thexmov %s, acc\n", lexeme);/*3*/
        match(HEX);
        break;
    case NUM:
        acc = strtod(lexeme, NULL);
        /*2*/printf("\tfltmov %s, acc\n", lexeme);/*2*/
        match(NUM);
        break;
    default:
        /*4*/strcpy(varname, lexeme);/*4*/
        match(ID);
        if (lookahead == ASGN) {
            match(ASGN); E();
            store(varname, acc);
            /*5*/printf("\tstore acc, %s\n", varname);/*5*/
        } else {
            acc = recall(varname);
        }
    }
}

void match(int expected)
{
    if (lookahead == expected) {
        lookahead = getToken(source);
    }
    else {
        fprintf(stderr, "token mismatch\n");
        exit(-3);
    }
}
