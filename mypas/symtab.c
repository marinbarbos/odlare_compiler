#include <symtab.h>

#define MAXSTBSIZE 0X100000
SYMTAB symtab[MAXSTBSIZE];

int somreFunction(void)
{
    SYMTAB newRegister;
    newRegister.objtype = 1;
    newRegister.lexlevel = 0;
    newRegister.parmflag = 0;
    return 0;
}
