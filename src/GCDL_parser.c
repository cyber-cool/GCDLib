#include "GCDL_parser.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include <stdio.h>

static int getLexeme(uchar **str, uchar *buff, size_t buff_sz, long *line, uchar **first_uchar, long *line_total);

static GCDL_object GCDL_parseStruct(uchar **str_ptr, uchar *name, long *line_ptr, GCDL_error **errors_array, size_t *errors_array_size, const char *filename);
static GCDL_object GCDL_parseField(uchar **str_ptr, uchar *name, long *line_ptr, GCDL_error **errors_array, size_t *errors_array_size, const char *filename);
static GCDL_object GCDL_parseArrayItem(uchar **str_ptr, uchar *lexbuff, int return_code, long *line_ptr, const char *filename);

static void addObject(GCDL_object **array, size_t *array_size, GCDL_object new_object);

static void guessAndAssignValue(unsigned char value_type, void **value, uchar *lexbuff);
static void guessAndAssignElement(unsigned char value_type, void **array, size_t array_sz, uchar *lexbuff);

static void reportError(GCDL_error **errors, size_t *errors_cnt, const char *description, int ID, long line, uchar *str, const char *filename);

static unsigned char guessValueType(uchar *value, int return_code);
static unsigned char valueTypeToVectorType(unsigned char value_type);

//   Return codes:
//    8      got vector end                      ')'
//    7      got vector definition               '('
//    6      got last structure member(string)   '}'
//    5      got field value (string)            ','
//    4      got field value                     ','
//    3      got field name                      ':'
//    2      got last structure member           '}'
//    1      got structure definition            '{'
//
//   -1      reached EOF

GCDL_file GCDL_parseFileStream(FILE *f_stream, const char *filename)
{
    // Reading the whole file to a string
    fseek(f_stream, 0, SEEK_END);
        long f_ln = ftell(f_stream);
    fseek(f_stream, 0, SEEK_SET);

    uchar *f_str = malloc(f_ln+sizeof(uchar));
    fread(f_str, 1, f_ln, f_stream);
    f_str[f_ln/2] = 0;

    fclose(f_stream);
    return GCDL_parseString(f_str, filename);
}
GCDL_file GCDL_parseString(uchar *str, const char *filename)
{
    uchar *str_ptr = str;

    int err = 0;
    long line = 1;

    long obj_def_line = -1;
    uchar *obj_def_str = NULL;
    uchar lexbuff[64];

    GCDL_object *obj_arr = malloc(sizeof(GCDL_object));
    size_t obj_arr_sz = 0;
    GCDL_error *err_arr = malloc(sizeof(GCDL_error));
    size_t err_arr_sz = 0;

    while((err = getLexeme(&str, lexbuff, 64, &obj_def_line, &obj_def_str, &line)) != -1)
    {
        switch(err)
        {
            case 1:
                addObject(&obj_arr, &obj_arr_sz, GCDL_parseStruct(&str, lexbuff, &line, &err_arr, &err_arr_sz, filename));
                break;
            case 3:
                addObject(&obj_arr, &obj_arr_sz, GCDL_parseField(&str, lexbuff, &line, &err_arr, &err_arr_sz, filename));
                break;
            case 7:
                --str_ptr;
                addObject(&obj_arr, &obj_arr_sz, GCDL_parseField(&str, lexbuff, &line, &err_arr, &err_arr_sz, filename));
                break;
        }
    }

    GCDL_file file;
        file.document_str = str_ptr;
        file.objects = obj_arr;
        file.objects_cnt = obj_arr_sz;
        file.errors = err_arr;
        file.error_cnt = err_arr_sz;

    return file;
}

static GCDL_object GCDL_parseStruct(uchar **str_ptr, uchar *name, long *line_ptr, GCDL_error **errors_array, size_t *errors_array_size, const char *filename)
{
    int err = 0;
    long obj_def_line = -1;
    uchar *obj_def_str = NULL;
    uchar lexbuff[64];

    GCDL_object struct_new;
    {
        memset(&struct_new, 0, sizeof(GCDL_object));
        struct_new.name = ustrcpy_alloc(name);
        struct_new.type = GCDL_OBJECT_STRUCTURE;

        struct_new.parser_info.filename = filename;
        struct_new.parser_info.line = *line_ptr;
        struct_new.parser_info.str_ptr = *str_ptr;
    }

    while((err = getLexeme(str_ptr, lexbuff, 64, &obj_def_line, &obj_def_str, line_ptr)) != -1)
    {
        switch(err)
        {
            case 2:
            case 6:
            {
                if(guessValueType(lexbuff, err) != GCDL_VALUE_UNKNOWN)
                { // it's turned out that it was an array
                    struct_new.type = GCDL_OBJECT_ARRAY;
                    addObject(&struct_new.children, (size_t*)&struct_new.children_count, GCDL_parseArrayItem(str_ptr, lexbuff, err, line_ptr, filename));
                }
                return struct_new;
            }break;
            case 1:
            {
                addObject(&struct_new.children, (size_t*)&struct_new.children_count, GCDL_parseStruct(str_ptr, lexbuff, line_ptr, errors_array, (size_t*)errors_array_size, filename));
            }break;
            case 3:
            {
                addObject(&struct_new.children, (size_t*)&struct_new.children_count, GCDL_parseField(str_ptr, lexbuff, line_ptr, errors_array, (size_t*)errors_array_size, filename));
            }break;

            case 4:
            case 5:
            {
                if(guessValueType(lexbuff, err) != GCDL_VALUE_UNKNOWN)
                { // it's turned out that it was an array
                    struct_new.type = GCDL_OBJECT_ARRAY;
                    addObject(&struct_new.children, (size_t*)&struct_new.children_count, GCDL_parseArrayItem(str_ptr, lexbuff, err, line_ptr, filename));
                }
            }break;

            case 7:
            { // Non-standard vector definition
                --(*str_ptr);
                addObject(&struct_new.children, (size_t*)&struct_new.children_count, GCDL_parseField(str_ptr, lexbuff, line_ptr, errors_array, (size_t*)errors_array_size, filename));
            }break;
        }
    }

    reportError(errors_array, errors_array_size,
                "Structure without an end", GCDL_ERROR_STRUCTURE_NO_END,
                obj_def_line, obj_def_str, filename);
    return struct_new;
}
static GCDL_object GCDL_parseField(uchar **str_ptr, uchar *name, long *line_ptr, GCDL_error **errors_array, size_t *errors_array_size, const char *filename)
{
    int err = 0;
    long obj_def_line = -1;
    uchar *obj_def_str = NULL;
    uchar lexbuff[64];

    int vector_end = 0;

    GCDL_object field_new;
    {
        memset(&field_new, 0, sizeof(GCDL_object));
        field_new.name = ustrcpy_alloc(name);
        field_new.type = GCDL_OBJECT_FIELD;

        field_new.parser_info.filename = filename;
        field_new.parser_info.line = *line_ptr;
        field_new.parser_info.str_ptr = *str_ptr;
    }

    while(1)
    {
        err = getLexeme(str_ptr, lexbuff, 64, &obj_def_line, &obj_def_str, line_ptr);

        switch(err)
        {
            case 4: case 5:
            {
                if(field_new.type == GCDL_OBJECT_FIELD){
                    unsigned char value_type = guessValueType(lexbuff, err);
                    field_new.value_type = value_type;
                    guessAndAssignValue(value_type, &field_new.value, lexbuff);

                    return field_new;
                }
                else if(vector_end == 0){
                    unsigned char value_type = guessValueType(lexbuff, err);
                    field_new.value_type = value_type;
                    guessAndAssignElement(value_type, &field_new.value, -field_new.children_count, lexbuff);
                    --field_new.children_count;
                }
                else{
                    field_new.value_type = valueTypeToVectorType(field_new.value_type);
                    return field_new;
                }
            }break;
            case 2: case 6:
            case -1:
            {
                // This field or vector is the last member of structure or file
                --(*str_ptr);
                if(field_new.type == GCDL_OBJECT_VECTOR){
                    field_new.value_type = valueTypeToVectorType(field_new.value_type);
                }
                else{
                    unsigned char value_type = guessValueType(lexbuff, err);
                    field_new.value_type = value_type;
                    guessAndAssignValue(value_type, &field_new.value, lexbuff);

                    return field_new;
                }
                return field_new;
            }break;

            case 7:
            { // it's turned out that it was a vector
                field_new.type = GCDL_OBJECT_VECTOR;
            }break;
            case 8:
            { // the vector end
                if(field_new.type == GCDL_OBJECT_VECTOR){
                    unsigned char value_type = guessValueType(lexbuff, err);
                    field_new.value_type = value_type;
                    guessAndAssignElement(value_type, &field_new.value, -field_new.children_count, lexbuff);
                    --field_new.children_count;

                    vector_end = 1;
                }
            }break;
        }
    }

    if(field_new.type == GCDL_OBJECT_FIELD){
        reportError(errors_array, errors_array_size,
                    "Field without an end", GCDL_ERROR_FIELD_NO_END,
                    obj_def_line, obj_def_str, filename);
    }
    else{
        reportError(errors_array, errors_array_size,
                    "Vector without an end", GCDL_ERROR_VECTOR_NO_END,
                    obj_def_line, obj_def_str, filename);
    }
    return field_new;
}
static GCDL_object GCDL_parseArrayItem(uchar **str_ptr, uchar *lexbuff, int return_code, long *line_ptr, const char *filename)
{
    GCDL_object array_item_new;
    memset(&array_item_new, 0, sizeof(GCDL_object));
    {
        array_item_new.name = str_to_ustr("");
        array_item_new.type = GCDL_OBJECT_ARRAY_ITEM;

        unsigned char value_type = guessValueType(lexbuff, return_code);
        array_item_new.value_type = value_type;
        guessAndAssignValue(value_type, &array_item_new.value, lexbuff);

        array_item_new.parser_info.filename = filename;
        array_item_new.parser_info.line = *line_ptr;
        array_item_new.parser_info.str_ptr = *str_ptr;
    }

    return array_item_new;
}
static int getLexeme(uchar **str, uchar *buff, size_t buff_sz, long *line, uchar **first_char, long *line_total)
{
    size_t lex_ln = 0;

    uchar c = 0;
    int waiting_comment = 0;

    int quotes = 0;
    int ever_quotes = 0;

    *line = -1;

    for(;(c = **str)
        && (((char)c != '{' && (char)c != '}' && (char)c != ':' && (char)c != ',' && (char)c != '(' && (char)c != ')')
        || quotes);
        ++*str)
    {
        if((char)c == '\\' && !quotes){
            if(!waiting_comment)
                waiting_comment = 1;
        }
        if((char)c == '"') {
            quotes = !quotes;
            ever_quotes = 1;
            continue;
        }
        if(!quotes){
            if(((char)c) == ' ' || ((char)c) == '\t' || ((char)c) == '\r')
                continue;
            if((char)c == '\n'){
                if(line){
                    ++*line_total;
                }
                waiting_comment = 0;
                continue;
            }
            if(waiting_comment){
                continue;
            }
        }

        if(lex_ln < buff_sz-1){
            if(*line == -1){
                *line = *line_total;
                *first_char = *str;
            }
            buff[lex_ln] = c;
            ++lex_ln;
        }
    }
    buff[lex_ln] = 0;

    ++*str;
    switch(c)
    {
        case '\0':      return -1;
        case '{' :      return  1;
        case '}' :      return  2;
        case ':' :      return  3;
        case ',' :      if(ever_quotes) return 5;
                        else            return 4;
        case '(':       return 7;
        case ')':       return 8;
    }

    return 0;
}

static void addObject(GCDL_object **array, size_t *array_size, GCDL_object new_object)
{
    *array = realloc(*array, sizeof(GCDL_object) * ((*array_size) + 1));
    (*array)[*array_size] = new_object;
    ++*array_size;
}

static void guessAndAssignValue(unsigned char value_type, void **value, uchar *lexbuff)
{
    switch(value_type)
    {
        case GCDL_VALUE_STRING:
            *value = ustrcpy_alloc(lexbuff);
            break;

        case GCDL_VALUE_LONG_INTEGER:
            *value = malloc(sizeof(long long));
            *((long long*)*value) = strtoll(ustr_to_str(lexbuff), NULL, 10);
            break;
        case GCDL_VALUE_INTEGER:
            *value = malloc(sizeof(int));
            *((int*)*value) = strtol(ustr_to_str(lexbuff), NULL, 10);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *value = malloc(sizeof(short));
            *((short*)*value) = strtol(ustr_to_str(lexbuff), NULL, 10);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *value = malloc(sizeof(uchar));
            *((uchar*)*value) = strtol(ustr_to_str(lexbuff), NULL, 10);
            break;

        case GCDL_VALUE_DOUBLE:
            *value = malloc(sizeof(double));
            *((double*)*value) = strtod(ustr_to_str(lexbuff), NULL);
            break;
        case GCDL_VALUE_FLOAT:
            *value = malloc(sizeof(float));
            *((float*)*value) = strtod(ustr_to_str(lexbuff), NULL);
            break;
    }
}
static void guessAndAssignElement(unsigned char value_type, void **array, size_t array_sz, uchar *lexbuff)
{
    char *str = ustr_to_str(lexbuff);
    switch(value_type)
    {
        case GCDL_VALUE_STRING:
            *array = realloc(*array, (array_sz+1) * sizeof(uchar*));
            ((uchar**)*array)[array_sz] = ustrcpy_alloc(lexbuff);
            break;

        case GCDL_VALUE_INTEGER:
            *array = realloc(*array, (array_sz+1) * sizeof(int));
            ((int*)*array)[array_sz] = strtol(str, NULL, 10);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *array = realloc(*array, (array_sz+1) * sizeof(short));
            ((short*)*array)[array_sz] = strtol(str, NULL, 10);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *array = realloc(*array, (array_sz+1) * sizeof(char));
            ((char*)*array)[array_sz] = strtol(str, NULL, 10);
            break;

        case GCDL_VALUE_DOUBLE:
            *array = realloc(*array, (array_sz+1) * sizeof(double));
            ((double*)*array)[array_sz] = strtod(str, NULL);
            break;
    }
    free(str);
}

static void reportError(GCDL_error **errors, size_t *errors_cnt, const char *description, int ID, long line, uchar *str, const char *filename)
{
    *errors = realloc(*errors, sizeof(GCDL_error) * ((*errors_cnt)+1));

    GCDL_error *error = &(*errors)[*errors_cnt];
        error->description = description;
        error->ID = ID;
        error->line = line;
        error->str_ptr = str;
        error->filename = filename;

    ++*errors_cnt;
}

static unsigned char guessValueType(uchar *value, int return_code)
{
    if(return_code == 5 || return_code == 6)
        return GCDL_VALUE_STRING;

    size_t value_ln = ustrlen(value);
    uchar value_end = value[value_ln-1];

    int contains_alpha = 0;
    int contains_digits = 0;
    size_t i;for(i = 0;i < value_ln;++i)
    {
        if(isalpha(value[i]))
            contains_alpha = 1;
        if(isdigit(value[i]))
            contains_digits = 1;
    }

    if(contains_digits && contains_alpha)
        switch(value_end)
        {
            case 'f':       return GCDL_VALUE_FLOAT;
            case 'd':       return GCDL_VALUE_DOUBLE;

            case 'b':       return GCDL_VALUE_BYTE_INTEGER;
            case 's':       return GCDL_VALUE_SHORT_INTEGER;
            case 'i':       return GCDL_VALUE_INTEGER;
            case 'l':       return GCDL_VALUE_LONG_INTEGER;
        }

    {
        if(value_ln >= 3
           && ((char)value[0]) == '\'' && ((char)value_end) == '\'')
            return GCDL_VALUE_CHARACTER;
    }

    // guessing number types without letters

    {
        uchar *guess_dot = ustrchr(value, '.');
        if(guess_dot && contains_digits && !contains_alpha)
            return GCDL_VALUE_DOUBLE;
    }

    if(contains_digits && !contains_alpha)
    {
        char *str = ustr_to_str(value);
        long long guess_int = strtoll(str, NULL, 10);
        if(guess_int > INT_MAX){
            free(str);
            return GCDL_VALUE_LONG_INTEGER;
        }
        else{
            free(str);
            return GCDL_VALUE_INTEGER;
        }
    }

    if(contains_alpha)
        return GCDL_VALUE_STRING;
    else
        return GCDL_VALUE_UNKNOWN;
}
static unsigned char valueTypeToVectorType(unsigned char value_type)
{
    switch(value_type)
    {
        case GCDL_VALUE_UNKNOWN           :return GCDL_VECTOR_UNKNOWN;

        case GCDL_VALUE_STRING            :return GCDL_VECTOR_STRING;
        case GCDL_VALUE_CHARACTER         :return GCDL_VECTOR_CHARACTER;

        case GCDL_VALUE_INTEGER           :return GCDL_VECTOR_INTEGER;
        case GCDL_VALUE_SHORT_INTEGER     :return GCDL_VECTOR_SHORT_INTEGER;
        case GCDL_VALUE_BYTE_INTEGER      :return GCDL_VECTOR_BYTE_INTEGER;
        case GCDL_VALUE_LONG_INTEGER      :return GCDL_VECTOR_LONG_INTEGER;

        case GCDL_VALUE_FLOAT             :return GCDL_VECTOR_FLOAT;
        case GCDL_VALUE_DOUBLE            :return GCDL_VECTOR_DOUBLE;
    }
    return GCDL_VECTOR_UNKNOWN;
}
