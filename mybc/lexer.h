#include <stdio.h>

#define MAXIDLEN 32

enum {
    ID = 1024,
    DEC,
    OCT,
    HEX,
    ASGN, // ASGN = ":="
} ;

extern int linenum;
extern int gettoken(FILE *);
extern char lexeme[];