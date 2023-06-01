#include <iostream>
#include <fstream>

#include <cstring>

#include <log.hpp>
#include <ml.hpp>

#include "../headers/def.hpp"
#include "../headers/converters.hpp"

int main(int argc, char** argv){
    char* input = 0;
    char* output = 0;
    output_format format = OUTPUT_TXT;

    //Load args
    argc--;
    argv++;
    for(int i = 0; i < argc; i++){
        if(STRCMP_EQ(argv[i], "-o") || STRCMP_EQ(argv[i], "--output")){
            i++;
            if(i >= argc)
                dtk::log::fatal_error("Excepted argument after '-o'.", 22); //EINVAL
            if(output)
                dtk::log::fatal_error("Output file already setted.", 22); //EINVAL
            
            output = argv[i];
        } else if(STRCMP_EQ(argv[i], "-xml")){
            format = OUTPUT_XML;
        } else if(STRCMP_EQ(argv[i], "-txt")){
            format = OUTPUT_TXT;
        } else{
            if(input)
                dtk::log::fatal_error("Input file already setted.", 22); //EINVAL
            
            input = argv[i];
        }
    }

    //Validate
    if(!input)
        dtk::log::fatal_error("No input file.", 22); //EINVAL
    
    if(!output){
        switch(format){
            default:
            case OUTPUT_TXT:
                output = "out.txt";
                break;
            case OUTPUT_XML:
                output = "out.xml";
                break;
        }
    }

    //Open input file
    FILE* input_file = fopen(input, "r");
    if(!input_file)
        dtk::log::fatal_error("Cannot open input file.", errno);
    
    //Load data
    dtk::ml::MLRoot* root = dtk::ml::ml_load_file(input_file);

    //Close input file
    fclose(input_file);

    //Create output
    std::ofstream file(output);
    if(!file.good())
        dtk::log::fatal_error("Cannot open output file.", -1);

    switch(format){
        case OUTPUT_TXT:
            ml_to_txt(file, root);
            break;
        case OUTPUT_XML:
            dtk::log::info("Not supported yet.");
            break;
    }

    file.close();

    //Free memory
    dtk::ml::ml_free_root(root);

    return 0;
}