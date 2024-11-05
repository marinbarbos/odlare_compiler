#pragma once

#include <stdio.h>
#include <stdlib.h>

extern FILE *src;

extern void match(int expected);
extern void mybc();

extern int lookahead;
extern double acc;
extern void E(void);
extern void T(void);
extern void F(void);
extern void R(void);
extern void Q(void);

void program(void); // initial symbol
void block(void);
void type(void);
void vardef(void);
void idlist(void);
void beginend(void);
void stmtlist(void);
void stmt(void);
void idstmt(void);
void exprlist(void);
void sbrgdef(void);
void parmlist(void);
void ifstmt(void);
void whilestmt(void);
void repeatstmt(void);
void expr(void);
void simpleexpr(void);
void term(void);
void factor(void);
void match(int);

extern int gettoken(FILE*);