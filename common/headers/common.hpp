#pragma once

#include <string>

namespace dtk::common{

    extern "C"{
        #include "common.h"
    }

    namespace str{
        void replase(std::string& s, const std::string& from, const std::string& to);
        void replase_all(std::string& s, const std::string& from, const std::string& to);
        void clean_beginning(std::string& s);

        struct CString{
            char* str = 0;

            void from(char* s);
            void from(const std::string& s);

            inline void as(char* s){ str = s; }

            ~CString(){
                free(str);
                str = 0;
            }
        };
    }
}