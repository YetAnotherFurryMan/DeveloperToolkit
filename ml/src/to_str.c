#include "../headers/ml.h"

#include <malloc.h>
#include <string.h>

#include <common.h>
#include <log.h>

char* ml_put(struct MLAttribute* d, char* str, char* prefix, char initializer, char* equalizer, char* terminator){
    size_t _begin_leng = strlen(prefix) + 1 + strlen(d->name); //{prefix}{initializer}{name}
    char* _begin = malloc(_begin_leng + 1);
    sprintf(_begin, "%s%c%s", prefix, initializer, d->name);

    size_t _value_leng = 0;
    char* _value = 0;
    if(d->value){
        char* val = d->value;
        
        if(val[strlen(val) - 1] == '\n'){
            val = ml_format_str(val);
            val[strlen(val) - 2] = 0;
        }

        _value_leng = strlen(equalizer) + 1 + strlen(val) + 1; //{equalizer}"{val}" || {equalizer}'{val}\n
        _value = malloc(_value_leng + 1);

        if(d->value[strlen(d->value) - 1] == '\n'){
            sprintf(_value, "%s\'%s\n", equalizer, val);
            free(val);
        } else{
            sprintf(_value, "%s\"%s\"", equalizer, val);
        }
    }

    size_t _terminator_leng = strlen(terminator);

    str = realloc(str, strlen(str) + _begin_leng + _value_leng + _terminator_leng);

    strcat(str, _begin);

    if(_value_leng)
        strcat(str, _value);
    
    if(_terminator_leng)
        strcat(str, terminator);
    
    free(_begin);
    free(_value);

    return str;
}

#define ml_put_definition(d, str, prefix) ml_put(d, str, prefix, '#', "=", "\n")
#define ml_put_attribute(a, str, prefix) ml_put(a, str, prefix, '!', ": ", "\n")
#define ml_put_modifier(m, str) ml_put(m, str, "", ' ', "=", "")

char* ml_put_value(char* v, char* str, char* prefix){
    size_t leng = strlen(str);

    char* val = v;
    if(val[strlen(val) - 1] == '\n'){
        val = ml_format_str(val);
        val[strlen(val) - 2] = 0;
    }

    leng += strlen(prefix) + 1 + strlen(val) + 2; // {prefix}"{val}" || {prefix}'{val}\n

    str = realloc(str, leng + 1);

    strcat(str, prefix);
    if(v[strlen(v) - 1] == '\n'){
        size_t pos = strlen(str);
        str[pos] = '\'';
        str[pos + 1] = 0;

        strcat(str, val);

        pos = strlen(str);
        str[pos] = '\n';
        str[pos + 1] = 0;
    } else{
        str = realloc(str, leng + 2);

        size_t pos = strlen(str);
        str[pos] = '\"';
        str[pos + 1] = 0;

        strcat(str, val);

        pos = strlen(str);
        str[pos] = '\"';
        str[pos + 1] = '\n';
        str[pos + 2] = 0;
    }

    return str;
}

char* ml_put_section(struct MLSection* s, char* str, char* prefix){
    size_t _prefix_leng = strlen(prefix);
    size_t _name_leng = strlen(s->name);

    //modifiers
    char* _modifiers = malloc(1);
    _modifiers[0] = 0;

    for(int i = 0; i < s->modifier_no; i++)
        _modifiers = ml_put_modifier(s->modifiers[i], _modifiers);
    
    //section begin
    char* _begin = malloc(_prefix_leng + 1 + _name_leng + strlen(_modifiers) + 3); //{prefix}<{name}{_modifiers}>\n\0
    sprintf(_begin, "%s<%s%s>\n", prefix, s->name, _modifiers);

    free(_modifiers);

    //new prefix
    char* _prefix = malloc(_prefix_leng + 2);
    sprintf(_prefix, "%s\t", prefix);

    char* _cnt = malloc(1);
    _cnt[0] = 0;

    //put definitions
    for(int i = 0; i < s->definition_no; i++)
        _cnt = ml_put_definition(s->definitions[i], _cnt, _prefix);

    //put attributes
    for(int i = 0; i < s->attribute_no; i++)
        _cnt = ml_put_attribute(s->attributes[i], _cnt, _prefix);
    
    //put sections
    for(int i = 0; i < s->section_no; i++)
        _cnt = ml_put_section(s->sections[i], _cnt, _prefix);
    
    //put values
    for(int i = 0; i < s->value_no; i++)
        _cnt = ml_put_value(s->values[i], _cnt, _prefix);

    free(_prefix);

    //section end
    size_t _end_leng = _prefix_leng + 2 + _name_leng + 2; //{prefix}</{name}>\n
    char* _end = malloc(_end_leng + 1);
    sprintf(_end, "%s</%s>\n", prefix, s->name);
    
    str = realloc(str, strlen(str) + strlen(_begin) + strlen(_cnt) + _end_leng + 1); //{str}{_begin}{_cnt}{_end}\0
    strcat(str, _begin);
    strcat(str, _cnt);
    strcat(str, _end);

    free(_begin);
    free(_cnt);
    free(_end);

    return str;
}

char* ml_root_to_str(struct MLRoot* r){
    char* str = malloc(1);
    strcpy(str, "");

    //put definitions
    for(int i = 0; i < r->definition_no; i++)
        str = ml_put_definition(r->definitions[i], str, "");
    
    //put attributes
    for(int i = 0; i < r->attribute_no; i++)
        str = ml_put_attribute(r->attributes[i], str, "");
    

    //put sections
    for(int i = 0; i < r->section_no; i++)
        str = ml_put_section(r->sections[i], str, "");

    return str;
}