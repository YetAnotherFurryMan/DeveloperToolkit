#include <stdio.h>
#include <stdlib.h>

void yyerror(char* msg, char* yytext, int lineno){
    fprintf(stderr, "At line %d:\n%s\n\tyytext:\'%s\'\n", lineno, msg, yytext);
    exit(-1);
}