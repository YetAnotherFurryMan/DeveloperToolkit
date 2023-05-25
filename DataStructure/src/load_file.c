#include "../headers/DTKDS.h"
#include "../headers/lexer.l.h"
#include "../headers/parser.y.h"

struct DSRoot* ds_get_root();

struct DSRoot* ds_load_file(FILE* f){
    ds_yyin = f;
    ds_yyparse();
    return ds_get_root();
}
