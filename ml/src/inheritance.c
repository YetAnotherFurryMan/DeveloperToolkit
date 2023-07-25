#define DTK_ML_INHERITANCE
#define DTK_ML_DTK_BUILDER
#include "../headers/ml.h"

#include <string.h>
#include <stdlib.h>

void ml_create_inheritance(MLSection* s){
    for(size_t i = 0; i < s->sections.no; i++){
        MLAttributeBuffer defs = {0, 0, 0};

        //Copy parent's definitions
        for(size_t j = 0; j < s->definitions.no; j++){
            ml_add_attribute(&defs, ml_copy_attribute(s->definitions.array[j]));
        }

        //Add child's definitions
        for(size_t j = 0; j < s->sections.array[i]->definitions.no; j++){
            ml_add_attribute(&defs, s->sections.array[i]->definitions.array[j]);
        }

        //Remove duplicats
        ml_deduplicate_attributes(&defs);

        //Free memory
        free(s->sections.array[i]->definitions.array);
        s->sections.array[i]->definitions = defs;

        //Create inheritance to child
        ml_create_inheritance(s->sections.array[i]);
    }
}

void ml_destroy_inheritance(MLSection* s, MLAttributeBuffer* b){
    //Destroy inheritance from children
    for(size_t i = 0; i < s->sections.no; i++){
        ml_destroy_inheritance(s->sections.array[i], &s->definitions);
    }

    //Break if s is the root
    if(b == 0)
        return;

    //Delete inheritanced definitions
    for(size_t j = 0; j < b->no; j++){
        for(size_t k = 0; k < s->definitions.no; k++){
            if(strcmp(b->array[j]->name, s->definitions.array[k]->name) == 0){
                char do_remove = 1;

                if(s->definitions.array[k]->value && b->array[j]->value)
                    if(strcmp(s->definitions.array[k]->value, b->array[j]->value) == 0)
                        do_remove++;
                else if(s->definitions.array[k]->value == b->array[j]->value)
                    do_remove++;

                if(do_remove){
                    ml_remove_attribute(&s->definitions, k);
                    break;
                }
            }
        }
    }
}

void ml_deduplicate_attributes(MLAttributeBuffer* b){
    for(size_t i = 0; i < b->no; i++){
        for(size_t j = i + 1; j < b->no; j++){
            if(strcmp(b->array[i]->name, b->array[j]->name) == 0){
                char* tmp = b->array[j]->value;
                b->array[j]->value = 0;
                free(b->array[i]->value);
                
                b->array[i]->value = tmp;
                ml_remove_attribute(b, j);

                j--;
            }
        }
    }
}