#include "../headers/ml.h"
#include "../headers/lexer.l.h"
#include "../headers/parser.y.h"

struct MLRoot* ml_get_root();

struct MLRoot* ml_load_file(FILE* f){
    ml_yyin = f;
    ml_yyparse();
    return ml_get_root();
}
