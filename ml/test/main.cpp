#include <iostream>
#include <fstream>

#define DTK_ML_HEADER_TEST
#include "../headers/ml.h"
#include "../headers/ml.hpp"

void to_str(dtk::ml::MLSection* r, char* save_to){
    std::cout << "To string";

    auto str = dtk::ml::ml_root_to_str(r);

    std::cout << "- end" << std::endl
              << "Printing result... ";

    if(save_to){
        std::ofstream output(save_to);
        output << str;
        output.close();
    } else{
        std::cout << str << std::endl;
    }

    free(str);

    std::cout << "- end" << std::endl;
}

int main(int argc, char** argv){
    struct dtk::ml::MLSection* r = 0;

    std::cout << "Loading input... ";

    if(--argc > 0){
        FILE* input = fopen(argv[1], "r");
        r = dtk::ml::ml_load_file(input);
        fclose(input);
    } else{
        r = dtk::ml::ml_load_file(0);
    }

    std::cout << "- end" << std::endl;

    to_str(r, (--argc > 0)?argv[2]:0);

    std::cout << "Create inheritance... ";

    dtk::ml::ml_create_inheritance(r);

    std::cout << "- end" << std::endl;

    to_str(r, (--argc > 0)?argv[3]:0);

    std::cout << "Destroy inheritance... ";

    dtk::ml::ml_destroy_inheritance(r, 0);

    std::cout << "- end" << std::endl;

    to_str(r, (--argc > 0)?argv[4]:0);
    
    std::cout << "Free... ";

    dtk::ml::ml_free_root(r);

    std::cout << "- end" << std::endl;

    return 0;
}