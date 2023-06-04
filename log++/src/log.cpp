#include "../headers/log.hpp"

#include <iostream>

namespace dtk::log{
    void terminate(int code){
        std::cerr << "Terminated." << std::endl;
        exit(code);
    }

    void error(const std::string& msg, int code){
        std::cerr << "Error: " << msg << std::endl;

        if(code != 0)
            terminate(code);
    }

    void fatal_error(const std::string& msg, int code){
        std::cerr << "Fatal ";
        error(msg, code);
    }

    void warning(const std::string& msg){
        std::cerr << "Warning: " << msg << std::endl;
    }


    void info(const std::string& msg){
        std::cout << "Info: " << msg << std::endl;
    }

    void assert(bool b, const std::string& msg, int code){
        if(b){
            std::cerr << "Assertion: " << msg << std::endl;
            
            if(code != 0)
                terminate(code);
        }
    }
}