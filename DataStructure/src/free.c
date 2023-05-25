#include "../headers/DTKDS.h"

#include <malloc.h>

void ds_free_attribute(struct DSAttribute* a){
    //free name
    if(a->name)
        free(a->name);
    
    //free value
    if(a->value)
        free(a->value);
    
    free(a);
}

void ds_free_section(struct DSSection* s){
    //free definitions
    for(int i = 0; i < s->definition_no; i++)
        ds_free_attribute(s->definitions[i]);
    
    free(s->definitions);

    //free attributes
    for(int i = 0; i < s->attribute_no; i++)
        ds_free_attribute(s->attributes[i]);

    free(s->attributes);

    //free modifiers
    for(int i = 0; i < s->modifier_no; i++)
        ds_free_attribute(s->modifiers[i]);

    free(s->modifiers);


    //free sections
    for(int i = 0; i < s->section_no; i++)
        ds_free_section(s->sections[i]);

    free(s->sections);

    //free values
    for(int i = 0; i < s->value_no; i++)
        free(s->values[i]);

    free(s->values);

    free(s);
}

void ds_free_root(struct DSRoot* r){
    //free definitions
    for(int i = 0; i < r->definition_no; i++)
        ds_free_attribute(r->definitions[i]);
    
    free(r->definitions);
    r->definitions = r->definition_no = 0;
    
    //free attributes
    for(int i = 0; i < r->attribute_no; i++)
        ds_free_attribute(r->attributes[i]);
    
    free(r->attributes);
    r->attributes = r->attribute_no = 0;

    //free sections
    for(int i = 0; i < r->section_no; i++)
        ds_free_section(r->sections[i]);
    
    free(r->sections);
    r->sections = r->section_no = 0;
}