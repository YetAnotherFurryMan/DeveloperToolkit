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

char* ml_format_str_put_hex_slash(char* s, size_t* size, int v){
    *size += 5; //{size} + \\x + {v}
    s = realloc(s, (*size));

    char buff[5]; //\\x{v}\0
    if(v > 15)
        sprintf(buff, "\\x%x", v);
    else
        sprintf(buff, "\\x0%x", v);

    strcat(s, buff);

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
        else if(c < 32 || c > 128)
            buf = ml_format_str_put_hex_slash(buf, &buf_size, (int)c);
        else{
            buf[strlen(buf) + 1] = 0;
            buf[strlen(buf)] = c;
        }
        
        s++;
    }

    return buf;
}