#include <ctype.h>
#include <stdio.h>
#include <lexer.h>

int linenum = 1;
// ID = [A-Za-z][A-Za-z0-9]*
// Ex: abacaXI123
int isID(FILE *tape)
{
	int head = getc(tape);

	if ( isalpha(head) ) {
		while ( isalnum(head = getc(tape)) );
		ungetc(head, tape);
		return ID;
	}

	ungetc(head, tape);
	return 0;
}

// DEC = [1-9][0-9]* | 0
// 1234567890
// 0012 = 0 0 12
int isDEC(FILE *tape)
{
	int head = getc(tape);

	if (isdigit(head)) {
		if (head == '0') {
			return DEC;
		}
		while(isdigit(head = getc(tape)));
		ungetc(head, tape);
		return DEC;
	}
	ungetc(head, tape);
	return 0;
}


void skipspaces(FILE *tape) {
	int head;

	while(isspace(head = getc(tape))) 
		if(head == '\n')
			linenum++;

	ungetc(head, tape);
}

// 1. Apresente uma expressão regular para definir números octais. O número 0 é octal? Justifique sua resposta.
// OCT = 0[0-7]*
// 012345670
// 0012
int isOCT(FILE *tape)
{
	int head = getc(tape);

	if (isdigit(head)) {
		if (head == '0') {
			while(isdigit(head = getc(tape))) {
				if(head > '7') {
					ungetc(head, tape);
					return 0;
				}
			}
			ungetc(head, tape);
			return OCT;
		}
	}
	ungetc(head, tape);
	return 0;
}

// 5. Análogo ao Exercício 2, implemente uma função predicado, int isHEX(FILE *tape), para decidir se uma cadeia, oriunda de tape, é ou não um hexadecimal.
// HEX = 0[xX][0-9A-F]+
// 0xFF
// 0x112
int isHEX(FILE *tape)
{
	int head = getc(tape);

	if (isdigit(head)) {
		if (head == '0' && (toupper(head = getc(tape)) == 'X')) {
			do {
				if(!(isalnum(head = getc(tape))) || head > 'F') {
					ungetc(head, tape);
					return 0;
				}
			} while (isalnum(head = getc(tape)));
			
			ungetc(head, tape);
			return HEX;
		}
	}
	ungetc(head, tape);
	return 0;
}











int gettoken(FILE *source)
{
	int token;
	skipspaces(source);
	if ((token = isHEX(source))) return token;
	if ((token = isID(source))) return token;
	if ((token = isOCT(source))) return token;
	if ((token = isDEC(source))) return token;
	token = getc(source);
	return token;
}