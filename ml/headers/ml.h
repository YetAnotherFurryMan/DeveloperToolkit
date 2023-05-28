#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>

#ifdef __cplusplus

namespace dtk::ml{
    extern "C"{

#endif //__cplusplus

struct MLAttribute{
    char* name;
    char* value;
};

struct MLSection{
    char* name;

    unsigned int definition_no;
    unsigned int attribute_no;
    unsigned int modifier_no;
    unsigned int section_no;
    unsigned int value_no;

    struct MLAttribute** definitions;
    struct MLAttribute** attributes;
    struct MLAttribute** modifiers;
    struct MLSection** sections;
    char** values;
};

struct MLRoot{
    unsigned int definition_no;
    unsigned int attribute_no;
    unsigned int section_no;

    struct MLAttribute** definitions;
    struct MLAttribute** attributes;
    struct MLSection** sections;
};

struct MLRoot* ml_load_file(FILE* f);

char* ml_root_to_str(struct MLRoot* r);
void ml_free_root(struct MLRoot* r);

#ifdef __cplusplus
    }
}
#endif //__cplusplus

#endif