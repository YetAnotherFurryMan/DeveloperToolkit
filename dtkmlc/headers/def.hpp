#pragma once

typedef enum{
    OUTPUT_TXT,
    OUTPUT_XML
} output_format;

#define STRCMP_EQ(a, b) (strcmp(a, b) == 0)