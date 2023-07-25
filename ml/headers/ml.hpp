#pragma once

#include "ml.h"

#undef ml_find_attribute
#undef ml_find_modifier
#undef ml_find_definition

#if defined(DTK_ML_STR_BUILDER)
    #undef ml_put_definition
    #undef ml_put_attribute
    #undef ml_put_modifier
#endif //DTK_ML_STR_BUILDER

#if defined(DTK_ML_BUFFERS)
    #undef ml_convert_from_attribute_buffer_ptr
    #undef ml_convert_from_section_buffer_ptr
    #undef ml_convert_from_value_buffer_ptr

    #undef ml_convert_from_attribute_buffer
    #undef ml_convert_from_section_buffer
    #undef ml_convert_from_value_buffer
#endif //DTK_ML_BUFFERS

#if defined(DTK_ML_DTK_BUILDER)
    #undef ml_copy_attribute
#endif //DTK_ML_DTK_BUILDER

#if defined(DTK_ML_ELEMENT_FREE)
    #undef ml_free_value
#endif //DTK_ML_ELEMENT_FREE

namespace dtk{
    namespace ml{
        inline size_t ml_find_attribute(MLSection* s, const char* name){
            return ml_find(&s->attributes, name);
        }

        inline size_t ml_find_modifier(MLSection* s, const char* name){
            return ml_find(&s->modifiers, name);
        }

        inline size_t ml_find_definition(MLSection* s, const char* name){
            return ml_find(&s->definitions, name);
        }

        #if defined(DTK_ML_STR_BUILDER)
            inline char* ml_put_definition(MLAttribute* d, char* str, const char* prefix){ 
                return ml_put(d, str, prefix, '#', "=", "\n");
            }

            inline char* ml_put_attribute(MLAttribute* a, char* str, const char* prefix){ 
                return ml_put(a, str, prefix, '!', ": ", "\n");
            }

            inline char* ml_put_modifier(MLAttribute* m, char* str){ 
                return ml_put(m, str, "", ' ', "=", "");
            }
        #endif //DTK_ML_STR_BUILDER

        #if defined(DTK_ML_BUFFERS)
            inline MLPointerBuffer ml_convert_from_attribute_buffer_ptr(MLAttributeBuffer* b){
                return {b->no, b->size, sizeof(MLAttribute), (void**)b->array};
            }

            inline MLPointerBuffer ml_convert_from_section_buffer_ptr(MLSectionBuffer* b){
                return {b->no, b->size, sizeof(MLSection), (void**)b->array};
            }

            inline MLPointerBuffer ml_convert_from_value_buffer_ptr(MLValueBuffer* b){
                return {b->no, b->size, sizeof(char), (void**)b->array};
            }

            inline MLPointerBuffer ml_convert_from_attribute_buffer(MLAttributeBuffer& b){
                return {b.no, b.size, sizeof(MLAttribute), (void**)b.array};
            }

            inline MLPointerBuffer ml_convert_from_section_buffer(MLSectionBuffer& b){
                return {b.no, b.size, sizeof(MLSection), (void**)b.array};
            }

            inline MLPointerBuffer ml_convert_from_value_buffer(MLValueBuffer& b){
                return {b.no, b.size, sizeof(char), (void**)b.array};
            }
        #endif //DTK_ML_BUFFERS

        #if defined(DTK_ML_DTK_BUILDER)
            inline MLAttribute* ml_copy_attribute(MLAttribute* a){
                return ml_new_attribute(a->name, a->value);
            }
        #endif //DTK_ML_DTK_BUILDER

        #if defined(DTK_ML_ELEMENT_FREE)
            inline void ml_free_value(char* v){
                free(v);
            }
        #endif //DTK_ML_ELEMENT_FREE
    }
}