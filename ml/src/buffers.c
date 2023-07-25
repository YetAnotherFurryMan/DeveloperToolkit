#define DTK_ML_BUFFERS
#include "../headers/ml.h"

#include <stdlib.h>
#include <string.h>

MLAttributeBuffer ml_convert_to_attribute_buffer(MLPointerBuffer* b){
    MLAttributeBuffer tmp = {0, 0, 0};

    if(b->type == sizeof(MLAttribute)){
        tmp.no = b->no;
        tmp.size = b->size;
        tmp.array = b->array;
    }
    
    return tmp;
}

MLSectionBuffer ml_convert_to_section_buffer(MLPointerBuffer* b){
    MLSectionBuffer tmp = {0, 0, 0};

    if(b->type == sizeof(MLSection)){
        tmp.no = b->no;
        tmp.size = b->size;
        tmp.array = b->array;
    }
    
    return tmp;
}

MLValueBuffer ml_convert_to_value_buffer(MLPointerBuffer* b){
    MLValueBuffer tmp = {0, 0, 0};

    if(b->type == sizeof(char*)){
        tmp.no = b->no;
        tmp.size = b->size;
        tmp.array = b->array;
    }
    
    return tmp;
}

void ml_add(MLPointerBuffer* b, void* e){
    b->no++;

    if(b->size == 0){
        b->size = sizeof(void*);
        b->array = malloc(b->size);
    } else{
        if(b->size < b->no * sizeof(void*)){
            b->size = b->no * sizeof(void*);
            b->array = realloc(b->array, b->size);
        }
    }

    b->array[b->no - 1] = e;
}

void* ml_remove(MLPointerBuffer* b, size_t index){
    if(index >= b->no)
        return 0;

    void* r = b->array[index];

    b->no--;
    b->array[index] = b->array[b->no];

    return r;
}

void ml_optimize(MLPointerBuffer* b){
    size_t size = sizeof(void*) * b->no;

    if(b->size > size){
        void** array = malloc(size);
        memcpy(array, b->array, size);

        free(b->array);

        b->size = size;
        b->array = array;
    }
}
