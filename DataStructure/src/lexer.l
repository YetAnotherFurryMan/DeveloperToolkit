%option noyywrap

%{
    #include <malloc.h>
    #include <ctype.h>

    #include "../headers/DTKDS.h"
    #include "../headers/parser.y.h"

    void yyerror(char* msg, char* yytext, int yylineno);
    #define yyerror(msg) yyerror(msg, yytext, yylineno)

    #define yybegin(x) BEGIN x

    #define hextoint(x) (isdigit((x)) ? (x) - '0' : ((x) - 'A') + 10)

    char* value = 0;
    unsigned long int value_leng = 0;
    void init_value();
    void putc_value(char c);
    void puts_value(char* s);
    void push_value();
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

\" { init_value(); yybegin(VALUE); }
\' { init_value(); yybegin(VALUE_LONG); }

<VALUE>[^\\"]+ { puts_value(yytext); }
<VALUE>\\\" { putc_value('\"'); }
<VALUE>\\\\ { putc_value('\\'); }
<VALUE>\\(x|X){hex} {
                        int temp = 0;
                        for(int i = 2; i > 0; i--){
                            temp  <<= 4;
                            temp  += hextoint(toupper(yytext[yyleng - i]));
                        }
                        putc_value((char)temp);
                    }
<VALUE>\\(u|U){hex}{hex} {
                             int temp = 0;
                             for(int i = 2; i > 0; i--){
                                 temp  <<= 4;
                                 temp  += hextoint(toupper(yytext[yyleng - i]));
                             }
                             putc_value((char)temp);
                             temp = 0;
                             for(int i = 4; i > 2; i--){
                                 temp  <<= 4;
                                 temp  += hextoint(toupper(yytext[yyleng - i]));
                             }
                             putc_value((char)temp);
                         }
<VALUE>\\n { putc_value('\n'); }
<VALUE><<EOF>> { yyerror("Lexical error: Unexpected EOF."); free(value); yybegin(INITIAL); }
<VALUE>\" { push_value(); yybegin(INITIAL); return TK_VALUE; }

<VALUE_LONG>. { putc_value(yytext[0]); }
<VALUE_LONG><<EOF>> { yyerror("Lexical error: Unexpected EOF."); free(value); yybegin(INITIAL); }
<VALUE_LONG>\n { putc_value('\n'); push_value(); yybegin(INITIAL); return TK_VALUE; }

\<\-\- { yybegin(COMMENT_A); }
\<\!\-\- { yybegin(COMMENT_B); }

<COMMENT_A>\-\-\> { yybegin(INITIAL); }
<COMMENT_A><<EOF>> { yyerror("Lexical error: Unexpected EOF."); yybegin(INITIAL); }

<COMMENT_B>. { yymore(); }
<COMMENT_B>\n { yymore(); }
<COMMENT_B>\-\-\> { yytext[yyleng - 3] = 0; printf("COMMENT_B: %s\n", yytext); yybegin(INITIAL); }
<COMMENT_B><<EOF>> { yyerror("Lexical error: Unexpected EOF."); yybegin(INITIAL); }

[  \t\n\r] { /*Skipp*/ }

. { yyerror("Lexical error: unknown character."); }

<<EOF>> { return 0; }

%%

void init_value(){
    value = malloc(1);
    value_leng = 1;
    strcpy(value, "");
}

void putc_value(char c){
    value[value_leng - 1] = c;
    value = realloc(value, ++value_leng);
    value[value_leng - 1] = 0;
}

void puts_value(char* s){
    size_t l = strlen(s);
    value = realloc(value, value_leng + l);
    strcat(value, s);
    value_leng += l;
}

void push_value(){
    yylval.TK_VALUE = malloc(value_leng);
    strcpy(yylval.TK_VALUE, value);
    free(value);

    value = 0;
    value_leng = 0;
}