#include <stdio.h>

#define MAXIDLEN 32

enum {
    ID = 1024,
    DEC,
    OCT,
    HEX,
    NUM,
    ASGN, // ASGN = ":=",
} ;

extern int linenum;
extern int getToken(FILE *);
extern char lexeme[MAXIDLEN+1];