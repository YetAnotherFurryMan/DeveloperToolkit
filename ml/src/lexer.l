%option noyywrap

%{
    #include <malloc.h>
    #include <ctype.h>

    #include "../headers/ml.h"
    #include "../headers/parser.y.h"

    void ml_yyerror(char* msg, char* yytext, int yylineno);
    #define yyerror(msg) ml_yyerror(msg, yytext, yylineno)
    #define yylval ml_yylval

    #define yybegin(x) BEGIN x

    #define hextoint(x) (isdigit((x)) ? (x) - '0' : ((x) - 'A') + 10)

    char* ml_value_lex = 0;
    unsigned long int ml_value_leng_lex = 0;
    void ml_init_value_lex();
    void ml_putc_value_lex(char c);
    void ml_puts_value_lex(char* s);
    void ml_push_value_lex();
%}

special [</>!#:=]
name [0-9A-Za-z_\\+&*.@~\(\)\[\]\{\}]+

hex [0-9a-fA-F]{2}

%x VALUE
%x VALUE_LONG

%x COMMENT_A
%x COMMENT_B

%%

{special} { return yytext[0]; }
{name}  { 
            yylval.TK_ID = malloc(yyleng + 1); 
            strcpy(yylval.TK_ID, yytext); 
            return TK_ID; 
        }

\" { ml_init_value_lex(); yybegin(VALUE); }
\' { ml_init_value_lex(); yybegin(VALUE_LONG); }

<VALUE>[^\\"]+ { ml_puts_value_lex(yytext); }
<VALUE>\\\" { ml_putc_value_lex('\"'); }
<VALUE>\\\\ { ml_putc_value_lex('\\'); }
<VALUE>\\n { ml_putc_value_lex('\n'); }
<VALUE>\\(x|X){hex} {
                        int temp = 0;
                        for(int i = 2; i > 0; i--){
                            temp  <<= 4;
                            temp  += hextoint(toupper(yytext[yyleng - i]));
                        }
                        ml_putc_value_lex((char)temp);
                    }
<VALUE>\\(u|U){hex}{hex} {
                             int temp = 0;
                             for(int i = 2; i > 0; i--){
                                 temp  <<= 4;
                                 temp  += hextoint(toupper(yytext[yyleng - i]));
                             }
                             ml_putc_value_lex((char)temp);
                             temp = 0;
                             for(int i = 4; i > 2; i--){
                                 temp  <<= 4;
                                 temp  += hextoint(toupper(yytext[yyleng - i]));
                             }
                             ml_putc_value_lex((char)temp);
                         }
<VALUE><<EOF>> { yyerror("Lexical error: Unexpected EOF."); free(ml_value_lex); yybegin(INITIAL); }
<VALUE>\" { ml_push_value_lex(); yybegin(INITIAL); return TK_VALUE; }

<VALUE_LONG>. { ml_putc_value_lex(yytext[0]); }
<VALUE_LONG><<EOF>> { yyerror("Lexical error: Unexpected EOF."); free(ml_value_lex); yybegin(INITIAL); }
<VALUE_LONG>\n { ml_putc_value_lex('\n'); ml_push_value_lex(); yybegin(INITIAL); return TK_VALUE; }

\<\-\- { yybegin(COMMENT_A); }
\<\!\-\- { yybegin(COMMENT_B); }

<COMMENT_A>. { /*Skipp*/ }
<COMMENT_A>\n { /*Skipp*/ }
<COMMENT_A>\-\-\> { yybegin(INITIAL); }
<COMMENT_A><<EOF>> { yyerror("Lexical error: Unexpected EOF."); yybegin(INITIAL); }

<COMMENT_B>. { yymore(); }
<COMMENT_B>\n { yymore(); }
<COMMENT_B>\-\-\> { /*yytext[yyleng - 3] = 0; printf("COMMENT_B: %s\n", yytext); !!!TODO: doc gen*/ yybegin(INITIAL); }
<COMMENT_B><<EOF>> { yyerror("Lexical error: Unexpected EOF."); yybegin(INITIAL); }

[  \t\n\r] { /*Skipp*/ }

. { yyerror("Lexical error: unknown character."); }

<<EOF>> { return 0; }

%%

void ml_init_value_lex(){
    ml_value_leng_lex = 1;
    ml_value_lex = malloc(1);
    ml_value_lex[0] = 0;
}

void ml_putc_value_lex(char c){
    ml_value_lex = realloc(ml_value_lex, ++ml_value_leng_lex);
    ml_value_lex[ml_value_leng_lex - 2] = c;
    ml_value_lex[ml_value_leng_lex - 1] = 0;
}

void ml_puts_value_lex(char* s){
    ml_value_leng_lex += strlen(s);
    ml_value_lex = realloc(ml_value_lex, ml_value_leng_lex);
    strcat(ml_value_lex, s);
}

void ml_push_value_lex(){
    yylval.TK_VALUE = malloc(ml_value_leng_lex);
    strcpy(yylval.TK_VALUE, ml_value_lex);
    free(ml_value_lex);

    ml_value_lex = 0;
    ml_value_leng_lex = 0;
}