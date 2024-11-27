#pragma once
#define MAXSTBSIZE 0x100000
#define MAXIDLEN 32
extern int symtab_next_entry;

// looks up a symbol appended to the symbol table (symtab)
int symtab_lookup(char const *, int);

// looks up a predefined symbol regarding possible conflict with
// the current symbol to append to the symbol table (symtab)
int symtab_append(char const *, int);

typedef struct _symtab_
{
    char name[MAXIDLEN];
    int objtype; // objtype == 0 if procedure == 1 if function == 2 if variable
    int type; // type == 0 if int32; == 1 if int64; == 2 if flt32; == 3 if flt64; == 4 if bool
    int lexlevel;
    int parmflag; // == 0 if variable; == 1 if paramter
    char offset[MAXIDLEN]; // variable/procedure/function address
} SYMTAB;

extern SYMTAB symtab[];