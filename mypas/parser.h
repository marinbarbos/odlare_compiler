#include <stdio.h>
#include <stdlib.h>

extern FILE *source;

extern void match(int expected);
extern void mybc();

extern int lookahead;
extern double acc;
extern void E(void);
extern void T(void);
extern void F(void);
extern void R(void);
extern void Q(void);

void program(void);
void block(void);
void vardef(void);
void sbprgdef(void);
void beginend(void);
void stmt(void);
void idstmt(void);
void ifstmt(void);
void whilestmt(void);
void repeatstmt(void);
void expr(void);
void simpleexpr(void);
void term(void);
void factor(void);
void exprlist(void);
void idlist(void);
void parmlist(void);
void type(void);

extern FILE *source;