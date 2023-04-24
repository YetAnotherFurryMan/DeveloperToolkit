#pragma once 

namespace project{
    typedef enum{
        PROJECT_TYPE_PROJECT,
        PROJECT_TYPE_SOLUTION
    } type;

    typedef enum{
        PROJECT_BUILD_MAKE,
        PROJECT_BUILD_BASH
    } build;

    typedef enum{
        PROJECT_ENABLE_LOG = 1,
        PROJECT_ENABLE_TESTS = 2
    } enable;

    typedef enum{
        PROJECT_EXTERN_BIN,
        PROJECT_EXTERN_LIB,
        PROJECT_EXTERN_INCLUDE
    } extern_type;

    typedef enum{
        PROJECT_EXPORT_BIN,
        PROJECT_EXPORT_LIB,
        PROJECT_EXPORT_INCLUDE
    } export_type;
}