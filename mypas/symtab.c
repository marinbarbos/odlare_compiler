#include <symtab.h>
#include <string.h>
#define MAXSTBSIZE 0x100000

SYMTAB symtab[MAXSTBSIZE];
int symtab_next_entry = 0;

int symtab_lookup(char const *lexleme, int lexlevel)
{
    for (int i = 0; i < symtab_next_entry; i++)
    {
        if (strcmp(symtab[i].name, lexeme) == 0 && symtab[i].lexlevel == lexlevel)
            return 1;
    }

    return 0;
}

int symtab_append(char const *lexleme, int lexlevel)
{
    int elementFound = symtab_lookup(lexeme, lexlevel);

    if (elementFound)
        return elementFound;

    // Adding new symbol
    strcpy(lexeme, symtab[symtab_next_entry].name);
    symtab[symtab_next_entry].lexlevel = lexlevel;

    // Incresing the new next free position
    symtab_next_entry++;

    return 0;
}
