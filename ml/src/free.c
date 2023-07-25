#define DTK_ML_ELEMENT_FREE
#include "../headers/ml.h"

#include <malloc.h>

void ml_free_attribute(MLAttribute* a){
    if(!a)
        return;
    
    //free name
    if(a->name)
        free(a->name);
    
    //free value
    if(a->value)
        free(a->value);
    
    free(a);
}

void ml_free_section(MLSection* s){
    if(!s)
        return;

    //free definitions
    for(int i = 0; i < s->definitions.no; i++)
        ml_free_attribute(s->definitions.array[i]);
    
    free(s->definitions.array);

    //free attributes
    for(int i = 0; i < s->attributes.no; i++)
        ml_free_attribute(s->attributes.array[i]);

    free(s->attributes.array);

    //free modifiers
    for(int i = 0; i < s->modifiers.no; i++)
        ml_free_attribute(s->modifiers.array[i]);

    free(s->modifiers.array);

    //free sections
    for(int i = 0; i < s->sections.no; i++)
        ml_free_section(s->sections.array[i]);

    free(s->sections.array);

    //free values
    for(int i = 0; i < s->values.no; i++)
        free(s->values.array[i]);

    free(s->values.array);

    free(s);
}

void ml_free_root(MLSection* r){
    if(!r)
        return;

    //free definitions
    for(int i = 0; i < r->definitions.no; i++)
        ml_free_attribute(r->definitions.array[i]);
    
    free(r->definitions.array);
    r->definitions.array = 0;
    r->definitions.no = 0;
    
    //free attributes
    for(int i = 0; i < r->attributes.no; i++)
        ml_free_attribute(r->attributes.array[i]);
    
    free(r->attributes.array);
    r->attributes.array = 0;
    r->attributes.no = 0;

    //free sections
    for(int i = 0; i < r->sections.no; i++)
        ml_free_section(r->sections.array[i]);
    
    free(r->sections.array);
    r->sections.array = 0;
    r->sections.no = 0;
}