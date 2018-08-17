#ifndef GCDL_STRUCTURES_H
#define GCDL_STRUCTURES_H

#include <stdlib.h>
#include <UString.h>

typedef struct GCDL_object GCDL_object;
struct GCDL_object
{
    unsigned char type;
        #define GCDL_OBJECT_STRUCTURE       1
        #define GCDL_OBJECT_FIELD           2

        #define GCDL_OBJECT_ARRAY           3
        #define GCDL_OBJECT_ARRAY_ITEM      4

        #define GCDL_OBJECT_VECTOR          5

    uchar *name;

    void *value;
    unsigned char value_type;
        #define GCDL_VALUE_UNKNOWN           0

        #define GCDL_VALUE_STRING            1
        #define GCDL_VALUE_CHARACTER         2

        #define GCDL_VALUE_INTEGER           13
        #define GCDL_VALUE_SHORT_INTEGER     14
        #define GCDL_VALUE_BYTE_INTEGER      15
        #define GCDL_VALUE_LONG_INTEGER      16

        #define GCDL_VALUE_FLOAT             36
        #define GCDL_VALUE_DOUBLE            37

        #define GCDL_VECTOR_UNKNOWN          100

        #define GCDL_VECTOR_STRING           101
        #define GCDL_VECTOR_CHARACTER        102

        #define GCDL_VECTOR_INTEGER          113
        #define GCDL_VECTOR_SHORT_INTEGER    114
        #define GCDL_VECTOR_BYTE_INTEGER     115
        #define GCDL_VECTOR_LONG_INTEGER     116

        #define GCDL_VECTOR_FLOAT            136
        #define GCDL_VECTOR_DOUBLE           137

    int children_count;
    GCDL_object *children;

    struct GCDL_parser_info
    {
        unsigned long line;
        uchar *str_ptr;
        const char *filename;
    }parser_info;
};

typedef struct GCDL_error GCDL_error;
struct GCDL_error
{
    const char *description;
    int ID;

    #define GCDL_ERROR_STRUCTURE_NO_END 10
    // "Structure without an end"
    #define GCDL_ERROR_FIELD_NO_END 11
    // "Field without an end"
    #define GCDL_ERROR_VECTOR_NO_END 12
    // "Vector without an end"

    long line;
    uchar *str_ptr;
    const char *filename;
};
typedef struct GCDL_file GCDL_file;
struct GCDL_file
{
    GCDL_object *objects;
    size_t objects_cnt;

    uchar *document_str;

    GCDL_error *errors;
    size_t error_cnt;
};

#endif
