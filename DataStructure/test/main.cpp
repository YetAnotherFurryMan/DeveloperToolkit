#include <iostream>
#include <vector>

#include "../headers/DTKDS.h"

extern "C"{
#include "../headers/lexer.l.h"
#include "../headers/parser.y.h"
struct dtk::ds::DSRoot* get_root();
}

int main(){
    std::cout << "Begin" << std::endl;
    yyparse();
    auto r = get_root();
    auto str = dtk::ds::ds_root_to_str(r);
    std::cout << "End" << std::endl;
    std::cout << "Root:" << std::endl;
    std::cout << str << std::endl;
    std::cout << "End of root" << std::endl;
    dtk::ds::ds_free_root(r);
}