#include <ctype.h>
#include <stdio.h>
#include <lexer.h>

char lexeme[MAXIDLEN+1];

int linenum = 1;

// ASGN = :=
// Ex: :=
int isASGN(FILE *tape)
{
	lexeme[0] = getc(tape);
	if (lexeme[0] == ':') {
		lexeme[1] = getc(tape);
		if (lexeme[1] == '=') {
			lexeme[2] = 0;
			return ASGN;
		}
        ungetc(lexeme[1], tape);
	}

	ungetc(lexeme[0], tape);
    lexeme[0] = 0;
	return 0;
}

// ID = [A-Za-z][A-Za-z0-9]*
// Ex: abacaXI123
int isID(FILE *tape)
{
	int i = 0;
	lexeme[i] = getc(tape);

	if ( isalpha(lexeme[i]) ) {
		i = 1;
		while ( isalnum(lexeme[i] = getc(tape))) {
			if(i < MAXIDLEN) i++;
		}
		ungetc(lexeme[i], tape);
        lexeme[i] = 0;
		return ID;
	}

	ungetc(lexeme[i], tape);
    lexeme[i] = 0;
	return 0;
}

// DEC = [1-9][0-9]* | 0
// 1234567890
// 0012 = 0 0 12
int isDEC(FILE *tape)
{
	int i = 0;
    lexeme[i] = getc(tape);

    if (isdigit(lexeme[i])) {
        if (lexeme[i] == '0') {
            return DEC;
        }
        i = 1;
        while (isdigit(lexeme[i] = getc(tape))) {
            i++;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return DEC;
    }

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
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
    int i = 0;
    lexeme[i] = getc(tape);

	if (isdigit(lexeme[i])) {
		if (lexeme[i] == '0') {
        	i = 1;
			while(isdigit(lexeme[i] = getc(tape))) {
				if(lexeme[i] > '7') {
					ungetc(lexeme[i], tape);
            		ungetc('0', tape);
					i--;
					lexeme[i] = 0;
					return 0;
				} else {
					i++;
				}
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return OCT;
		}
	}
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
	return 0;
}

// 5. Análogo ao Exercício 2, implemente uma função predicado, int isHEX(FILE *tape), para decidir se uma cadeia, oriunda de tape, é ou não um hexadecimal.
// HEX = 0[xX][0-9A-F]+
// 0xFF
// 0x112
int isHEX(FILE *tape)
{
	int i = 0;
	lexeme[i] = getc(tape);

	if (isdigit(lexeme[i])) {
		i = 1;
		if (lexeme[i] == '0' && (toupper(lexeme[i] = getc(tape)) == 'X')) {
			do {
				if(!(isalnum(lexeme[i] = getc(tape))) || lexeme[i] > 'F') {
					ungetc(lexeme[i], tape);
					lexeme[i] = 0;
					return 0;
				}
			} while (isalnum(lexeme[i] = getc(tape)));
			
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return HEX;
		}
	}
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
	return 0;
}


double lexval;
int isNUM(FILE *tape)
{
	int charCount;
	charCount = fscanf(tape, "%s", &lexval);
	if (charCount) return NUM;
	else return 0;

}

/* FP = DEC.[0-9]* | .[0-9]+ */
int isFP(FILE *tape)
{
    int i;
    if (isDEC(tape)) {
        i = strlen(lexeme);
        if ((lexeme[i] = getc(tape)) != '.') {
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return DEC;
        }

        i++;
        while (isdigit(lexeme[i] = getc(tape))) {
            i++;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return 1;
    }

    i = 0;
    if ((lexeme[i] = getc(tape)) == '.') {
        i++;
        if (! isdigit(lexeme[i] = getc(tape))) {
            ungetc(lexeme[i], tape);
            ungetc('.', tape);
            i--;
            lexeme[i] = 0;
            return 0;
        }

        i++;
        while (isdigit(lexeme[i] = getc(tape))) {
            i++;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return 1;
    } 

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/* EE = [eE]['+''-']?[0-9]+ */ 
int isEE(FILE *tape) 
{
    int i = strlen(lexeme);
    int e, sign;
    
    if (toupper(lexeme[i] = getc(tape)) == 'E') {
        e = lexeme[i];
        i++;
        if ((lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-') {
            sign = lexeme[i];
            i++;
            if (! isdigit(lexeme[i] = getc(tape))) {
                ungetc(lexeme[i], tape);
                ungetc(sign, tape);
                ungetc(e, tape);
                i -= 2;
                lexeme[i] = 0;
                return 0;
            }

            i++;
            while (isdigit(lexeme[i] = getc(tape))) {
                i++;
            }
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return 1;
        }    

        if (! isdigit(lexeme[i])) {
            ungetc(lexeme[i], tape);
            ungetc(e, tape);
            i--;
            lexeme[i] = 0;
            return 0;
        }

        i++;
        while (isdigit(lexeme[i] = getc(tape))) {
            i++;
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return 1;
    }

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/* FLT = FP EE? | DEC EE */
int isFLT(FILE *tape)
{
    int fp;

    if ((fp = isFP(tape)) == DEC) {
        if (! isEE(tape)) 
            return DEC;
        return NUM;

    } else if (fp) {
        isEE(tape);
        return NUM;
    }

    return 0;
}

int gettoken(FILE *source)
{
	int token;
	skipspaces(source);
	if ((token = isHEX(source))) return token;
	if ((token = isID(source))) return token;
	if ((token = isOCT(source))) return token;
    if ((token = isNUM(source))) return token;
	if ((token = isDEC(source))) return token;
    if ((token = isASGN(source))) return token; 
	token = getc(source);
	return token;
}