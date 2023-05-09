#include "../headers/common.hpp"

#include <cstring>

namespace dtk::common{
    namespace str{
        void replase(std::string& s, const std::string& from, const std::string& to){
            if(s.empty() || from.empty())
                return;
            
            auto pos = s.find(from);
            if(pos != std::string::npos)
                s.replace(pos, from.length(), to);
        }

        void replase_all(std::string& s, const std::string& from, const std::string& to){
            if(s.empty() || from.empty())
                return;
        
            std::size_t pos = 0;

            while((pos = s.find(from, pos)) != std::string::npos){
                s.replace(pos, from.length(), to);
                pos += to.length();
            }
        }

        void clean_beginning(std::string& s){
            //remove spacies and tabs from line begining
            int x = 0;
    
            while(s[x] == ' ' || s[x] == '\t')
                x++;
                
            s = s.substr(x);
        }

        //class CString
        
        void CString::from(char* s){
            str = new char[std::strlen(s)];
            std::strcpy(str, s);
        }

        void CString::from(const std::string& s){
            str = new char[s.length()];
            std::strcpy(str, s.c_str());
        }
    }
}