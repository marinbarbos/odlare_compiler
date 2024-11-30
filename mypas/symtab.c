#include <symtab.h>
#include <string.h>

int symtab_next_entry = 0;
SYMTAB symtab[MAXSTBSIZE];

int symtab_lookup(char const *lexeme, int lexlevel)
{
    for (int i = 0; i < symtab_next_entry; i++)
    {
        if (strcmp(symtab[i].name, lexeme) == 0 && symtab[i].lexlevel == lexlevel)
            return 1;
    }

    return 0;
}

int symtab_append(char const *lexeme, int lexlevel)
{
    int elementFound = symtab_lookup(lexeme, lexlevel);

    if (elementFound)
        return elementFound;

    // Adding new symbol
    strcpy(symtab[symtab_next_entry].name, lexeme);
    symtab[symtab_next_entry].lexlevel = lexlevel;

    // Incresing the new next free position
    symtab_next_entry++;

    return 0;
}
