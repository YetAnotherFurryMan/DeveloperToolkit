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
        PROJECT_ITEM_BIN,
        PROJECT_ITEM_LIB,
        PROJECT_ITEM_INC,
        PROJECT_ITEM_RES,
        PROJECT_ITEM_BIN_DIR,
        PROJECT_ITEM_LIB_DIR,
        PROJECT_ITEM_INC_DIR,
        PROJECT_ITEM_RES_DIR
    } item_type;

    typedef enum{
        PROJECT_RULE_121 = 2,
        PROJECT_RULE_12N = 3,
        PROJECT_RULE_N21 = 0,
        PROJECT_RULE_N2N = 1
    } rule_type;
}