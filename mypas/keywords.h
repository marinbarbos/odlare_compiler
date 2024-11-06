#pragma once

enum
{
    BEGIN = 0x1000000,
    PROGRAM,
    VAR,
    IF,
    WHILE,
    REPEAT,
    PROCEDURE,
    FUNCTION,
    THEN,
    ELSE,
    DO,
    UNTIL,
    IN,
    OR,
    DIV,
    MOD,
    AND,
    NOT,
    INTEGER,
    REAL,
    DOUBLE,
    BOOLEAN,
    END,
};

extern char *keyword[];

int iskeyword(char*);