#include "../headers/common.h"

#include <malloc.h>
#include <string.h>

char* common_str_copy(const char* s){
    size_t len = strlen(s);

    char* tmp = malloc(len + 1);
    memcpy(tmp, s, len);
    tmp[len] = 0;

    return tmp;
}