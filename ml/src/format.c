#include "../headers/ml.h"

#include <malloc.h>
#include <string.h>

char* ml_format_str_put_slash(char* s, size_t* size, char c){
    *size += 2; //{size} + \\ + {c}

    s = realloc(s, (*size)); 

    size_t pos = strlen(s);
    s[pos] = '\\';
    s[pos + 1] = c;
    s[pos + 2] = 0;

    return s;
}

char* ml_format_str_puts_slash(char* s, size_t* size, char* v){
    *size += 1 + strlen(v); //{size} + \\ + {strlen(v)}
    s = realloc(s, (*size));

    size_t pos = strlen(s);
    s[pos] = '\\';
    s[pos + 1] = 0;
    strcat(s, v);

    return s;
}

char* ml_format_str(char* s){
    size_t buf_size = strlen(s) + 1; //Minimal size
    char* buf = malloc(buf_size);
    strcpy(buf, "");

    //Transform each character to format
    while(*s){
        char c = *s;

        if(c == '\'' || c == '\"' || c == '\\')
            buf = ml_format_str_put_slash(buf, &buf_size, c);
        else if(c == '\n')
            buf = ml_format_str_put_slash(buf, &buf_size, 'n');
        else if(c == '\t')
            buf = ml_format_str_put_slash(buf, &buf_size, 't');
        else{
            buf[strlen(buf) + 1] = 0;
            buf[strlen(buf)] = c;
        }
        
        s++;
    }

    return buf;
}