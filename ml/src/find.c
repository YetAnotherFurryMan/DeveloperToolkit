#include "../headers/ml.h"

#include <string.h>

size_t ml_find(const MLAttributeBuffer* b, const char* name){
    for(size_t i = 0; i < b->no; i++){
        if(strcmp(b->array[i]->name, name) == 0)
            return i;
    }

    return -1;
}

size_t ml_find_section(const MLSectionBuffer* b, const char* name){
    for(size_t i = 0; i < b->no; i++){
        if(strcmp(b->array[i]->name, name) == 0)
            return i;
    }

    return -1;
}

size_t ml_find_value(const MLValueBuffer* b, const char* name){
    for(size_t i = 0; i < b->no; i++){
        if(strcmp(b->array[i], name) == 0)
            return i;
    }

    return -1;
}