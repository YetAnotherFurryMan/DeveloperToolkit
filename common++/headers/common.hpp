#pragma once

#include <string>
#include <string_view>

#include <cstring>

namespace dtk::common{
    namespace str{
        struct CString{
            char* m_str = 0;
            size_t m_allocated = 0;

            CString() = default;
            CString(const CString& cstr);
            CString(CString&& cstr);

            CString(char* str): m_str{str}, m_allocated{strlen(str) + 1} {}
            CString(char* str, size_t alloc): m_str{str}, m_allocated{alloc} {}

            ~CString();

            void from(const char* s);
            void from(const std::string& s);
            void from(const std::string_view& s);

            //inline char* str(){ return m_str; }
            inline const char* str() const { return m_str; }
            inline size_t size() const { return m_allocated; }
            inline size_t length() const { return strlen(m_str); }
            inline bool has_value() const { return m_allocated > 0; }

            void resizie(size_t size);
            void free();
        };

        bool replase(std::string& s, const std::string& from, const std::string& to);
        unsigned long replase_all(std::string& s, const std::string& from, const std::string& to);
        void clean_beginning(std::string& s);
    }
}