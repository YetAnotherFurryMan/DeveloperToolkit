#include <iostream>
#include <fstream>

#include "../headers/ml.h"

int main(int argc, char** argv){
    struct dtk::ml::MLRoot* r = 0;

    std::cout << "Loading input... ";

    if(--argc > 0){
        FILE* input = fopen(argv[1], "r");
        r = dtk::ml::ml_load_file(input);
        fclose(input);
    } else{
        r = dtk::ml::ml_load_file(0);
    }

    std::cout << "- end" << std::endl
              << "To string... ";

    auto str = dtk::ml::ml_root_to_str(r);

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

    dtk::ml::ml_free_root(r);

    std::cout << "- end" << std::endl;
}