#include <ctype.h>
#include <stdio.h>
#include <lexer.h>

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

int getToken(FILE *source)
{
	if (isID(source)) printf("It's an ID\n");
	else if (isDEC(source)) printf("It's decimal integer\n");
	else printf("Not ID nor DEC\n");
}
