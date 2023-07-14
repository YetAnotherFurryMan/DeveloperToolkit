#include "../headers/common.hpp"

namespace dtk::common::str{
    bool replase(std::string& s, const std::string& from, const std::string& to){
        if(s.empty() || from.empty())
            return false;
        
        auto pos = s.find(from);
        if(pos != std::string::npos){
            s.replace(pos, from.length(), to);
            return true;
        }

        return false;
    }

    unsigned long replase_all(std::string& s, const std::string& from, const std::string& to){
        if(s.empty() || from.empty())
                return 0;
    
        std::size_t pos = 0;
        unsigned long count = 0;
        while((pos = s.find(from, pos)) != std::string::npos){
            s.replace(pos, from.length(), to);
            pos += to.length();
            count++;
        }

        return count;
    }

    void clean_beginning(std::string& s){
        int x = 0;
    
        while(s[x] == ' ' || s[x] == '\t')
            x++;
                
        s = s.substr(x);
    }

}