#include <string.h>

enum {
    BEGIN = 0X10000,
    PROGRAM,
    END
};

extern char *keywords[];

char *keywords[] = {
    "begin",
    "program",
    "end"
}

int iskeyword(char *word) {
    for (int i = BEGIN; i <= END; i++) {
        if (strcmp(word, keyword[i - BEGIN])) {
            return i;
        }
    }
    return 0;
}