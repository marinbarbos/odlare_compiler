#include <symtab.h>
#include <string.h>

int symtab_next_entry = 0;
SYMTAB symtab[MAXSTBSIZE];

/* Função que busca um símbolo na tabela de símbolos, num determinado nível léxico.
   Retorna 1 caso encontre e 0 caso não encontre */
int symtab_lookup(char const *lexeme, int lexlevel)
{
    for (int i = 0; i < symtab_next_entry; i++)
    {
        if (strcmp(symtab[i].name, lexeme) == 0 && symtab[i].lexlevel == lexlevel)
            return 1;
    }

    return 0;
}

/* Função que tenta inserir um novo símbolo na tabela de símbolos, primeiro validando se ele já não existe
   no nível léxico em questão */
int symtab_append(char const *lexeme, int lexlevel)
{
    int elementFound = symtab_lookup(lexeme, lexlevel);

    if (elementFound)
        return elementFound;

    // Adiciona um novo símbolo
    strcpy(symtab[symtab_next_entry].name, lexeme);
    symtab[symtab_next_entry].lexlevel = lexlevel;

    // Incrementa a próxima posição disponível
    symtab_next_entry++;

    return 0;
}
