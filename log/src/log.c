#include "../headers/log.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void log_terminate(int code){
    puts("Terminated.\n");
    exit(code);
}

void log_error(const char* msg, int code){
    printf("Error: %s\n", msg);

    if(code != 0)
        log_terminate(code);
}

void log_fatal_error(const char* msg, int code){
    puts("Fatal ");
    log_error(msg, code);
}

void log_warning(const char* msg){
    printf("Warning: %s\n", msg);
}

void log_info(const char* msg){
    printf("Info: %s\n", msg);
}

void log_assert(char b, const char* msg, int code){
    if(b){
        printf("Assertion: %s\n", msg);

        if(code != 0)
            log_terminate(code);
    }
}

void log_assert_errno(){
    log_assert(errno != 0, strerror(errno), errno);
}
