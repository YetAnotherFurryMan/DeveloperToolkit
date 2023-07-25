#define DTK_ML_DTK_BUILDER
#define DTK_ML_BUFFERS
#define DTK_ML_ELEMENT_FREE
#include "../headers/ml.h"

#include <string.h>
#include <stdlib.h>

MLAttribute* ml_new_attribute(const char* name, const char* value){
    MLAttribute* a = malloc(sizeof(MLAttribute));

    a->name = malloc(strlen(name) + 1);
    strcpy(a->name, name);

    a->value = 0;

    if(value){
        a->value = malloc(strlen(value) + 1);
        strcpy(a->value, value);
    }

    return a;
}

MLSection* ml_new_section(){
    MLSection* s = malloc(sizeof(MLSection));

    s->name = 0;

    s->values.no = 0;
    s->values.size = 0;
    s->values.array = 0;

    s->sections.no = 0;
    s->sections.size = 0;
    s->sections.array = 0;

    s->modifiers.no = 0;
    s->modifiers.size = 0;
    s->modifiers.array = 0;

    s->attributes.no = 0;
    s->attributes.size = 0;
    s->attributes.array = 0;

    s->definitions.no = 0;
    s->definitions.size = 0;
    s->definitions.array = 0;

    return s;
}

#define _ML_ADD(b, e, tmp) { ml_add(&tmp, e); b->no = tmp.no; b->size = tmp.size; b->array = tmp.array; }

void ml_add_attribute(MLAttributeBuffer* b, MLAttribute* a){
    MLPointerBuffer tmp = ml_convert_from_attribute_buffer_ptr(b);
    _ML_ADD(b, a, tmp);
}

void ml_add_section(MLSectionBuffer* b, MLSection* s){
    MLPointerBuffer tmp = ml_convert_from_section_buffer_ptr(b);
    _ML_ADD(b, s, tmp);
}

void ml_add_value(MLValueBuffer* b, char* v){
    MLPointerBuffer tmp = ml_convert_from_value_buffer_ptr(b);
    _ML_ADD(b, v, tmp);
}

#undef _ML_ADD

#define _ML_REMOVE(b, index, tmp, free) { void* r = ml_remove(&tmp, index); b->no = tmp.no; b->array = tmp.array; free(r); }

void ml_remove_attribute(MLAttributeBuffer* b, size_t index){
    MLPointerBuffer tmp = ml_convert_from_attribute_buffer_ptr(b);
    _ML_REMOVE(b, index, tmp, ml_free_attribute);
}

void ml_remove_section(MLSectionBuffer* b, size_t index){
    MLPointerBuffer tmp = ml_convert_from_section_buffer_ptr(b);
    _ML_REMOVE(b, index, tmp, ml_free_section);
}

void ml_remove_value(MLValueBuffer* b, size_t index){
    MLPointerBuffer tmp = ml_convert_from_value_buffer_ptr(b);
    _ML_REMOVE(b, index, tmp, ml_free_value);
}

#undef _ML_REMOVE

#define _ML_OPTIMIZE(b, tmp) { ml_optimize(&tmp); b->size = tmp.size; b->array = tmp.array; }

void ml_optimize_attributes(MLAttributeBuffer* b){
    MLPointerBuffer tmp = ml_convert_from_attribute_buffer_ptr(b);
    _ML_OPTIMIZE(b, tmp);
}

void ml_optimize_sections(MLSectionBuffer* b){
    MLPointerBuffer tmp = ml_convert_from_section_buffer_ptr(b);
    _ML_OPTIMIZE(b, tmp);
}

void ml_optimize_values(MLValueBuffer* b){
    MLPointerBuffer tmp = ml_convert_from_value_buffer_ptr(b);
    _ML_OPTIMIZE(b, tmp);
}

#undef _ML_OPTIMIZE
