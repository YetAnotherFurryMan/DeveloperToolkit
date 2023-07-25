#ifndef _ML_H_
#define _ML_H_

#if defined(DTK_ML_HEADER_TEST)
    #define DTK_ML_INHERITANCE
    #define DTK_ML_STR_BUILDER
    #define DTK_ML_BUFFERS
    #define DTK_ML_DTK_BUILDER
    #define DTK_ML_ELEMENT_FREE
#endif //DTK_ML_HEADER_TEST

#include <stdio.h>

#ifdef __cplusplus

namespace dtk::ml{
    extern "C"{

#endif //__cplusplus

typedef struct MLValueBuffer{
    size_t no;
    size_t size;
    const char** array;
} MLValueBuffer;

typedef struct MLAttribute{
    char* name;
    char* value;
} MLAttribute;

typedef struct MLAttributeBuffer{
    size_t no;
    size_t size;
    MLAttribute** array;
} MLAttributeBuffer;

struct MLSection;
typedef struct MLSectionBuffer{
    size_t no;
    size_t size;
    struct MLSection** array;
} MLSectionBuffer;

typedef struct MLSection{
    char* name;

    MLValueBuffer values;
    MLSectionBuffer sections;
    MLAttributeBuffer modifiers;
    MLAttributeBuffer attributes;
    MLAttributeBuffer definitions;
} MLSection;

MLSection* ml_load_file(FILE* f);

char* ml_root_to_str(MLSection* r);
void ml_free_root(MLSection* r);

char* ml_format_str(char* s);

size_t ml_find(const MLAttributeBuffer* b, const char* name);
size_t ml_find_section(const MLSectionBuffer* b, const char* name);
size_t ml_find_value(const MLValueBuffer* b, const char* name);

#define ml_find_attribute(s, name) ml_find(&s->attributes, name)
#define ml_find_modifier(s, name) ml_find(&s->modifiers, name)
#define ml_find_definition(s, name) ml_find(&s->definitions, name)

#if defined(DTK_ML_INHERITANCE)
    void ml_create_inheritance(MLSection* s);
    void ml_destroy_inheritance(MLSection* s, MLAttributeBuffer* b);
    void ml_deduplicate_attributes(MLAttributeBuffer* b);
#endif //DTK_ML_INHERITANCE

#if defined(DTK_ML_STR_BUILDER)
    char* ml_put(MLAttribute* d, char* str, const char* prefix, const char initializer, const char* equalizer, const char* terminator);
    char* ml_put_section(MLSection* s, char* str, const char* prefix);
    char* ml_put_value(const char* v, char* str, const char* prefix);

    #define ml_put_definition(d, str, prefix) ml_put(d, str, prefix, '#', "=", "\n")
    #define ml_put_attribute(a, str, prefix) ml_put(a, str, prefix, '!', ": ", "\n")
    #define ml_put_modifier(m, str) ml_put(m, str, "", ' ', "=", "")
#endif //DTK_ML_STR_BUILDER

#if defined(DTK_ML_BUFFERS)
    typedef struct MLPointerBuffer{
        size_t no;
        size_t size;
        size_t type;
        void** array;
    } MLPointerBuffer;

    MLAttributeBuffer ml_convert_to_attribute_buffer(MLPointerBuffer* b);
    MLSectionBuffer ml_convert_to_section_buffer(MLPointerBuffer* b);
    MLValueBuffer ml_convert_to_value_buffer(MLPointerBuffer* b);

    void ml_add(MLPointerBuffer* b, void* element);
    void* ml_remove(MLPointerBuffer* b, size_t index);
    void ml_optimize(MLPointerBuffer* b);

    #define ml_convert_from_attribute_buffer_ptr(b) {b->no, b->size, sizeof(MLAttribute), b->array}
    #define ml_convert_from_section_buffer_ptr(b) {b->no, b->size, sizeof(MLSection), b->array}
    #define ml_convert_from_value_buffer_ptr(b) {b->no, b->size, sizeof(char), b->array}

    #define ml_convert_from_attribute_buffer(b) {b.no, b.size, sizeof(MLAttribute), b.array}
    #define ml_convert_from_section_buffer(b) {b.no, b.size, sizeof(MLSection), b.array}
    #define ml_convert_from_value_buffer(b) {b.no, b.size, sizeof(char), b.array}
#endif //DTK_ML_BUFFERS

#if defined(DTK_ML_DTK_BUILDER)
    MLAttribute* ml_new_attribute(const char* name, const char* value);
    MLSection* ml_new_section();

    void ml_add_attribute(MLAttributeBuffer* b, MLAttribute* a);
    void ml_add_section(MLSectionBuffer* b, MLSection* s);
    void ml_add_value(MLValueBuffer* b, char* v);

    void ml_remove_attribute(MLAttributeBuffer* b, size_t index);
    void ml_remove_section(MLSectionBuffer* b, size_t index);
    void ml_remove_value(MLValueBuffer* b, size_t index);

    void ml_optimize_attributes(MLAttributeBuffer* b);
    void ml_optimize_sections(MLSectionBuffer* b);
    void ml_optimize_values(MLValueBuffer* b);

    #define ml_copy_attribute(a) ml_new_attribute(a->name, a->value)
#endif //DTK_ML_DTK_BUILDER

#if defined(DTK_ML_ELEMENT_FREE)
    void ml_free_attribute(MLAttribute* a);
    void ml_free_section(MLSection* s);

    #define ml_free_value(v) free(v)
#endif //DTK_ML_ELEMENT_FREE

#ifdef __cplusplus
    }
}
#endif //__cplusplus

#endif //_ML_H_