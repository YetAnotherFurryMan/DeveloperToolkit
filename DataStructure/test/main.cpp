#include <iostream>
#include <fstream>

#include "../headers/DTKDS.h"

int main(int argc, char** argv){
    struct dtk::ds::DSRoot* r = 0;

    std::cout << "Loading input... ";

    if(--argc > 0){
        FILE* input = fopen(argv[1], "r");
        r = dtk::ds::ds_load_file(input);
        fclose(input);
    } else{
        r = dtk::ds::ds_load_file(0);
    }

    std::cout << "- end" << std::endl
              << "To string... ";

    auto str = dtk::ds::ds_root_to_str(r);

    std::cout << "- end" << std::endl
              << "Printing result... ";

    if(--argc > 0){
        std::ofstream output(argv[2]);
        output << str;
        output.close();
    } else{
        std::cout << str << std::endl;
    }

    std::cout << "- end" << std::endl
              << "Free... ";

    dtk::ds::ds_free_root(r);

    std::cout << "- end" << std::endl;
}