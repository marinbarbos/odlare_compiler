#pragma once

#include <lexer.h>

// looks up a symbol appended to the symbol table (symtab)
int symtab_lookup(char const *symbol);
// looks up a predefined symbol regarding possible conflitct with
//  the current symbol to append to the symbol table (symtab)
int symtab_append(char const *, int);

typedef struct _symtab_
{
    char *name[MAXIDLEN];
    int objtype; // objtype == 0 if procedure; == 1 if function; == 2 if variable
    int type;    // type == 0 if int32; == 1 if int64; == 2 if flt32; == 3 if flt64; == 4 if bool
    int lexlevel;
    int parmflag; // == 0 if variable; == 1 if parameter
} SYMTAB;

extern SYMTAB symtab[];