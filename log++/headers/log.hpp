#pragma once

#include <string>

#include <cerrno>
#include <cstring>

namespace dtk::log{
    void terminate(int code);
    void error(const std::string& msg, int code = 0);
    void fatal_error(const std::string& msg, int code = 0);
    void warning(const std::string& msg);
    void info(const std::string& msg);

    #ifdef DTK_LOG_ENABLE_ASSERTION

    void assert(bool b, const std::string& msg, int code = 0);
    inline void assert_errno(){
        assert(errno != 0, strerror(errno), errno);
    }

    #else

    inline void assert(bool b, const std::string& msg, int code = 0){}
    inline void assert_errno(){}

    #endif
}