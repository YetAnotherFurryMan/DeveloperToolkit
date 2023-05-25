#ifndef _DTK_DTKDS_H_
#define _DTK_DTKDS_H_

#include <stdio.h>

#ifdef __cplusplus

namespace dtk::ds{
    extern "C"{

#endif //__cplusplus

struct DSAttribute{
    char* name;
    char* value;
};

struct DSSection{
    char* name;

    unsigned int definition_no;
    unsigned int attribute_no;
    unsigned int modifier_no;
    unsigned int section_no;
    unsigned int value_no;

    struct DSAttribute** definitions;
    struct DSAttribute** attributes;
    struct DSAttribute** modifiers;
    struct DSSection** sections;
    char** values;
};

struct DSRoot{
    unsigned int definition_no;
    unsigned int attribute_no;
    unsigned int section_no;

    struct DSAttribute** definitions;
    struct DSAttribute** attributes;
    struct DSSection** sections;
};

struct DSRoot* ds_load_file(FILE* f);

char* ds_root_to_str(struct DSRoot* r);
void ds_free_root(struct DSRoot* r);

#ifdef __cplusplus
    }
}
#endif //__cplusplus

#endif //_DTK_DTKDS_H_