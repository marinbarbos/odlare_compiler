#include <keywords.h>
#include <string.h>

char *keyword[] = {
    "begin",
    "end",
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
    "not"
};

int iskeyword(char *word)
{
    for (int i = BEGIN; i <= END; i++)
    {
        if (strcmp(word, keyword[i - BEGIN]))
        {
            return i;
        }
    }
}