#include <stdio.h>

#define MAXIDLEN 32

enum
{
    QUIT = 0,
    EXIT = 1,
    ID = 1024,
    DEC,
    OCT,
    HEX,
    NUM,
    ASGN, // ASGN = ":=",
    FLT,  
    EXP,
    UC
};

extern int linenum;
extern int gettoken(FILE *);
extern char lexeme[MAXIDLEN + 1];