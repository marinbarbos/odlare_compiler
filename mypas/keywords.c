#include <keywords.h>
#include <string.h>

char *keyword[] = {
    "begin",
    "program",
    "var",
    "if",
    "while",
    "repeat",
    "procedure",
    "function",
    "then",
    "else",
    "do",
    "until",
    "in",
    "or",
    "div",
    "mod",
    "and",
    "not",
    "integer",
    "real",
    "double",
    "boolean",
    "end",
};

int iskeyword(char *word)
{
    for (int i = BEGIN; i <= END; i++) // end precisa ficar no final por causa disso
    {
        if (strcmp(word, keyword[i - BEGIN]))
        {
            return i;
        }
    }
}