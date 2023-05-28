#include <stdio.h>
#include <stdlib.h>

void ml_yyerror(char* msg, char* yytext, int lineno){
    fprintf(stderr, "At line %d:\n%s\n\tyytext:\'%s\'\n", lineno, msg, yytext);
    exit(-1);
}