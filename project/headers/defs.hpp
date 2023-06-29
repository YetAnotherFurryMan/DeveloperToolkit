#pragma once 

#define PROJECT_FILE ".project/project"
#define TEMPLATES_FILE ".project/templates.dtk"
#define MODULES_FILE ".project/modules.dtk"

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
        PROJECT_EXPORT_BIN,
        PROJECT_EXPORT_LIB,
        PROJECT_EXPORT_INCLUDE,
        PROJECT_EXPORT_RES
    } export_type;
}