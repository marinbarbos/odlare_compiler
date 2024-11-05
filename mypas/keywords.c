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
    "integer",
    "real",
    "double",
    "word",
    "char",
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
    "end"};

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