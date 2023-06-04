#include "../headers/common.hpp"

#include <utility>

#include <malloc.h>

namespace dtk::common::str{
    CString::CString(const CString& cstr){
        strcpy(m_str, cstr.m_str);
        m_allocated = cstr.m_allocated;
    }

    CString::CString(CString&& cstr){
        m_str = cstr.m_str;
        m_allocated = cstr.m_allocated;

        cstr.m_str = 0;
        cstr.m_allocated = 0;
    }

    CString::~CString(){
        this->free();
    }

    void CString::from(const char* s){
        if(has_value())
            this->free();
        
        m_allocated = strlen(s) + 1;

        m_str = (char*)malloc(m_allocated);
        memcpy(m_str, s, m_allocated);
    }

    void CString::from(const std::string& s){
        if(has_value())
            this->free();
        
        m_allocated = s.length() + 1;

        m_str = (char*)malloc(m_allocated);
        memcpy(m_str, s.c_str(), m_allocated);
    }

    void CString::from(const std::string_view& s){
        if(has_value())
            this->free();
        
        m_allocated = s.length() + 1;

        m_str = (char*)malloc(m_allocated);
        memcpy(m_str, s.data(), m_allocated);
    }

    void CString::resizie(size_t size){
        if(size == m_allocated)
            return;
        
        if(size < m_allocated){
            char* str = (char*)malloc(size);
            memcpy(str, m_str, size - 1);
            str[size - 1] = 0;

            this->free();

            m_str = str;
            m_allocated = size;
        } else{
            m_allocated = size;
            m_str = (char*)realloc(m_str, m_allocated);
        }
    }

    void CString::free(){
        ::free(m_str);

        m_str = 0;
        m_allocated = 0;
    }

}