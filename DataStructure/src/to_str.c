#include "../headers/DTKDS.h"

#include <malloc.h>
#include <string.h>

char* ds_put_definition(struct DSAttribute* d, char* str){
    size_t leng = strlen(str);
    leng += 1 + strlen(d->name) + 1; //#{name}\n
    if(d->value)
        leng += 1 + strlen(d->value) + 1; //="{value}" --only if value exists
    
    str = realloc(str, leng);

    strcat(str, "#");
    strcat(str, d->name);

    if(d->value){
        strcat(str, "=\"");
        strcat(str, d->value);
        strcat(str, "\"");
    }

    strcat(str, "\n");

    return str;
}

char* ds_put_varible(struct DSAttribute* v, char* str){
    size_t leng = strlen(str);
    leng += 1 + strlen(v->name) + 3 + strlen(v->value) + 2; //#{name}: "{value}"\n

    str = realloc(str, leng);

    strcat(str, "#");
    strcat(str, v->name);
    strcat(str, ": \"");
    strcat(str, v->value);
    strcat(str, "\"\n");

    return str;
}

char* ds_put_modifier(struct DSAttribute* m, char* str){
    size_t leng = strlen(str);
    leng += 1 + strlen(m->name) + 2 + strlen(m->value) + 1; // {name}="{value}"

    str = realloc(str, leng);

    strcat(str, " ");
    strcat(str, m->name);
    strcat(str, "=\"");
    strcat(str, m->value);
    strcat(str, "\"");

    return str;
}

char* ds_put_attribute(struct DSAttribute* a, char* str, char* prefix){
    size_t leng = strlen(str);
    leng += strlen(prefix) + 1 + strlen(a->name) + 3 + strlen(a->value) + 2; //{prefix}!{name}: "{value}"\n

    str = realloc(str, leng);

    strcat(str, prefix);
    strcat(str, "!");
    strcat(str, a->name);
    strcat(str, ": \"");
    strcat(str, a->value);
    strcat(str, "\"\n");

    return str;
}

char* ds_put_value(char* v, char* str, char* prefix){
    size_t leng = strlen(str);
    leng += strlen(prefix) + 1 + strlen(v) + 2; // {prefix}"{value}"\n

    str = realloc(str, leng);

    strcat(str, prefix);
    strcat(str, "\"");
    strcat(str, v);
    strcat(str, "\"\n");

    return str;
}

char* ds_put_section(struct DSSection* s, char* str, char* prefix){
    size_t prefix_leng = strlen(prefix);

    //Section begin
    size_t leng = strlen(str);
    leng += prefix_leng + 1 + strlen(s->name); //{prefix}<{name}
    str = realloc(str, leng);

    strcat(str, prefix);
    strcat(str, "<");
    strcat(str, s->name);
    
    //Put modifiers
    for(int i = 0; i < s->modifier_no; i++)
        str = ds_put_modifier(s->modifiers[i], str);

    //End of modifiers
    leng = strlen(str);
    leng += 2; //>\n
    str = realloc(str, leng);
    strcat(str, ">\n");

    //New prefix
    char* _prefix = malloc(prefix_leng + 2);
    strcpy(_prefix, prefix);
    strcat(_prefix, "\t");

    //Put attributes
    for(int i = 0; i < s->attribute_no; i++)
        str = ds_put_attribute(s->attributes[i], str, _prefix);
    
    //Put sections
    for(int i = 0; i < s->section_no; i++)
        str = ds_put_section(s->sections[i], str, _prefix);
    
    //Put values
    for(int i = 0; i < s->value_no; i++)
        str = ds_put_value(s->values[i], str, _prefix);

    free(_prefix);

    //Section end
    leng = strlen(str);
    leng += prefix_leng + 2 + strlen(s->name) + 2; //{prefix}</{name}>\n
    str = realloc(str, leng);

    strcat(str, prefix);
    strcat(str, "</");
    strcat(str, s->name);
    strcat(str, ">\n");

    return str;
}

char* ds_root_to_str(struct DSRoot* r){
    char* str = malloc(1);
    size_t str_leng = 1;
    strcpy(str, "");

    //put definitions
    for(int i = 0; i < r->definition_no; i++)
        str = ds_put_definition(r->definitions[i], str);

    //put varibles
    for(int i = 0; i < r->varible_no; i++)
        str = ds_put_varible(r->varibles[i], str);
    
    //put attributes
    for(int i = 0; i < r->attribute_no; i++)
        str = ds_put_attribute(r->attributes[i], str, "");
    

    //put sections
    for(int i = 0; i < r->section_no; i++)
        str = ds_put_section(r->sections[i], str, "");

    return str;
}