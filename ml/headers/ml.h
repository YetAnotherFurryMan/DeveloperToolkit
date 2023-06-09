#ifndef _ML_H_
#define _ML_H_

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

char* ml_format_str(char* s);

#if defined(DTK_ML_STR_BUILDER)
    char* ml_put(struct MLAttribute* d, char* str, char* prefix, char initializer, char* equalizer, char* terminator);
    char* ml_put_section(struct MLSection* s, char* str, char* prefix);
    char* ml_put_value(char* v, char* str, char* prefix);

    #define ml_put_definition(d, str, prefix) ml_put(d, str, prefix, '#', "=", "\n")
    #define ml_put_attribute(a, str, prefix) ml_put(a, str, prefix, '!', ": ", "\n")
    #define ml_put_modifier(m, str) ml_put(m, str, "", ' ', "=", "")
#endif //DTK_ML_STR_BUILDER

#if defined(DTK_ML_DTK_BUILDER)
    struct MLAttribute* ml_new_attribute(char* name, char* value);
    struct MLSection* ml_new_section();

    void ml_add_definition(struct MLSection* s, struct MLAttribute* d);
    void ml_add_attribute(struct MLSection* s, struct MLAttribute* a);
    void ml_add_modifier(struct MLSection* s, struct MLAttribute* m);
    void ml_add_section(struct MLSection* s, struct MLSection* ss);
    void ml_add_value(struct MLSection* s, char* v);
#endif //DTK_ML_DTK_BUILDER

#ifdef __cplusplus
    }
}
#endif //__cplusplus

#endif //_ML_H_