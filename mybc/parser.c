#include <lexer.h>
#include <parser.h>
int lookahead;

#include <string.h>
#define SYMTABSIZE 0x100 // 256
double acc;
char symtab[SYMTABSIZE][MAXIDLEN+1];
double vmeme[SYMTABSIZE];
int symtab_next_entry = 0;

int lookup(char* varname) {
    int i;
	for (i = 0; i < symtab_next_entry; i++)
	{
		if (strcmp(symtab[i],varname) == 0) break;
	}
    if( i == symtab_next_entry) {
        strcpy(symtab[i], varname);
    }
    return i;
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
        printf ("\tneg acc\n");
        signal = 0;
    }
    /*1*/
    
    /*2*/
    if(oplus) {
        switch(oplus) {
            case '+': {
                printf ("\tadd acc, stack[sp]\n", oplus);
                break;
            }
            case '-': {
                printf ("\tsub stack[sp], acc\n", oplus);
                break;
            }
        }
        oplus = 0;
    }
    /*2*/

    if(lookahead == '+' || lookahead == '-') {
        /*3*/
        oplus = lookahead;
        printf("\tpush acc\n");
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
            printf("\tmul stack[sp], acc\n", otimes);
            break;
        }
        case '/': {
            printf("\tdiv stack[sp], acc\n", otimes);
            break;
        }
    }
    otimes = 0;
    /*4*/

    if (lookahead == '*' || lookahead == '/') {
        /*5*/
        otimes = lookahead;
        printf("\tpush acc\n");
        /*5*/
        match(lookahead);
        goto _F;
    }
}


/* F -> ID | DEC | OCT | HEX | FLT | ( E ) */
void F(void)
{
    switch(lookahead) {
    case '(':
        match('('); 
        E(); 
        match(')');
        break;
    case DEC:
        /*6*/printf("\tmov %s, acc\n", lexeme);/*6*/
        match(DEC);
        break;
    case OCT:
        /*6*/printf("\tsasdasdasdasd %s, acc\n", lexeme);/*6*/
        match(OCT);
        break;
    case HEX:
        /*6*/printf("\taaaaaaaaaaaaaaaaa %s, acc\n", lexeme);/*6*/
        match(HEX);
        break;
    default:
        /*7*/printf(" %s ", lexeme);/*7*/
        match(ID);
        if (lookahead == ASGN) {
            match(ASGN); E();
        }
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
 /*
 
 
double recall(char* var) {
    int i;
	for (i = 0; i < symtab_next_entry; i++) {
		if (strcmp(symtab[i], var) == 0) return vmeme[i];
	}
	strcpy(symtab[symtab_next_entry], var);
	symtab_next_entry++;
	return 0.0;
}

*/