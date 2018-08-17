#include "GCDL_iterator.h"

static void __GCDL_iterate_over_array(GCDL_object *array, size_t array_sz,
                       int depth,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector);


// File procedures
void GCDL_iterate_over_file(GCDL_file *file,
                       int depth,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector)
{
    if(depth == 0)
        return;

    size_t i;for(i = 0;i < file->objects_cnt;++i)
    {
        if(file->objects[i].children && file->objects[i].children_count > 0)
            __GCDL_iterate_over_array(file->objects[i].children, file->objects[i].children_count, depth-1,
                                        foreach_structure, foreach_field, foreach_array, foreach_arrayItem, foreach_vector);

        if(foreach_structure)       foreach_structure(&file->objects[i]);
        if(foreach_field)           foreach_field(&file->objects[i]);
        if(foreach_array)           foreach_array(&file->objects[i]);
        if(foreach_arrayItem)       foreach_arrayItem(&file->objects[i]);
        if(foreach_vector)          foreach_vector(&file->objects[i]);
    }
}
static void __GCDL_iterate_over_array(GCDL_object *array, size_t array_sz,
                       int depth,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector)
{
    if(depth == 0)
        return;

    size_t i;for(i = 0;i < array_sz;++i)
    {
        if(array[i].children && array[i].children_count > 0)
            __GCDL_iterate_over_array(array[i].children, array[i].children_count, depth-1,
                                        foreach_structure, foreach_field, foreach_array, foreach_arrayItem, foreach_vector);

        if(foreach_structure)       foreach_structure(&array[i]);
        if(foreach_field)           foreach_field(&array[i]);
        if(foreach_array)           foreach_array(&array[i]);
        if(foreach_arrayItem)       foreach_arrayItem(&array[i]);
        if(foreach_vector)          foreach_vector(&array[i]);
    }
}

void GCDL_iterate_over_file_recursively(GCDL_file *file,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector)
{
    GCDL_iterate_over_file(file, -1,
                      foreach_structure, foreach_field, foreach_array, foreach_arrayItem, foreach_vector);
}
void GCDL_iterate_over_file_root(GCDL_file *file,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector)
{
     GCDL_iterate_over_file(file, 1,
                      foreach_structure, foreach_field, foreach_array, foreach_arrayItem, foreach_vector);
}


// Array procedures
void GCDL_iterate_over_array(GCDL_object *array, size_t array_size,
                       int depth,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector)
{
    __GCDL_iterate_over_array(array, array_size,
                              depth,
                              foreach_structure, foreach_field, foreach_array, foreach_arrayItem, foreach_vector);
}
void GCDL_iterate_over_array_recursively(GCDL_object *array, size_t array_size,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector)
{
    GCDL_iterate_over_array(array, array_size,
                            -1,
                            foreach_structure, foreach_field, foreach_array, foreach_arrayItem, foreach_vector);
}
void GCDL_iterate_over_array_root(GCDL_object *array, size_t array_size,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector)
{
    GCDL_iterate_over_array(array, array_size,
                            1,
                            foreach_structure, foreach_field, foreach_array, foreach_arrayItem, foreach_vector);
}

// Useful parsing tools

#include <GCDL_parser.h>
void GCDL_assign_object_valuel(GCDL_object *object, long long *destination)
{
    switch(object->value_type)
    {
        case GCDL_VALUE_LONG_INTEGER:
            *((long long*)destination) = *((long long*)object->value);
            break;
        case GCDL_VALUE_INTEGER:
            *((long long*)destination) = *((int*)object->value);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *((long long*)destination) = *((short*)object->value);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *((long long*)destination) = *((char*)object->value);
            break;

        case GCDL_VALUE_DOUBLE:
            *((long long*)destination) = *((double*)object->value);
            break;
        case GCDL_VALUE_FLOAT:
            *((long long*)destination) = *((float*)object->value);
            break;
    }
}
void GCDL_assign_object_valuei(GCDL_object *object, int *destination)
{
    switch(object->value_type)
    {
        case GCDL_VALUE_LONG_INTEGER:
            *((int*)destination) = *((long long*)object->value);
            break;
        case GCDL_VALUE_INTEGER:
            *((int*)destination) = *((int*)object->value);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *((int*)destination) = *((short*)object->value);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *((int*)destination) = *((char*)object->value);
            break;

        case GCDL_VALUE_DOUBLE:
            *((int*)destination) = *((double*)object->value);
            break;
        case GCDL_VALUE_FLOAT:
            *((int*)destination) = *((float*)object->value);
            break;
    }
}
void GCDL_assign_object_values(GCDL_object *object, short *destination)
{
    switch(object->value_type)
    {
        case GCDL_VALUE_LONG_INTEGER:
            *((short*)destination) = *((long long*)object->value);
            break;
        case GCDL_VALUE_INTEGER:
            *((short*)destination) = *((int*)object->value);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *((short*)destination) = *((short*)object->value);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *((short*)destination) = *((char*)object->value);
            break;

        case GCDL_VALUE_DOUBLE:
            *((short*)destination) = *((double*)object->value);
            break;
        case GCDL_VALUE_FLOAT:
            *((short*)destination) = *((float*)object->value);
            break;
    }
}
void GCDL_assign_object_valueb(GCDL_object *object, unsigned char *destination)
{
    switch(object->value_type)
    {
        case GCDL_VALUE_LONG_INTEGER:
            *((char*)destination) = *((long long*)object->value);
            break;
        case GCDL_VALUE_INTEGER:
            *((char*)destination) = *((int*)object->value);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *((char*)destination) = *((short*)object->value);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *((char*)destination) = *((char*)object->value);
            break;

        case GCDL_VALUE_DOUBLE:
            *((char*)destination) = *((double*)object->value);
            break;
        case GCDL_VALUE_FLOAT:
            *((char*)destination) = *((float*)object->value);
            break;
    }
}
void GCDL_assign_object_valued(GCDL_object *object, double *destination)
{
    switch(object->value_type)
    {
        case GCDL_VALUE_LONG_INTEGER:
            *((double*)destination) = *((long long*)object->value);
            break;
        case GCDL_VALUE_INTEGER:
            *((double*)destination) = *((int*)object->value);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *((double*)destination) = *((short*)object->value);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *((double*)destination) = *((char*)object->value);
            break;

        case GCDL_VALUE_DOUBLE:
            *((double*)destination) = *((double*)object->value);
            break;
        case GCDL_VALUE_FLOAT:
            *((double*)destination) = *((float*)object->value);
            break;
    }
}
void GCDL_assign_object_valuef(GCDL_object *object, float *destination)
{
    switch(object->value_type)
    {
        case GCDL_VALUE_LONG_INTEGER:
            *((float*)destination) = *((long long*)object->value);
            break;
        case GCDL_VALUE_INTEGER:
            *((float*)destination) = *((int*)object->value);
            break;
        case GCDL_VALUE_SHORT_INTEGER:
            *((float*)destination) = *((short*)object->value);
            break;
        case GCDL_VALUE_BYTE_INTEGER:
            *((float*)destination) = *((char*)object->value);
            break;

        case GCDL_VALUE_DOUBLE:
            *((float*)destination) = *((double*)object->value);
            break;
        case GCDL_VALUE_FLOAT:
            *((float*)destination) = *((float*)object->value);
            break;
    }
}

void GCDL_assign_object_vectori(GCDL_object *object, int *destination)
{
    int vec_ln = -object->children_count;
    switch(object->value_type)
    {
        case GCDL_VECTOR_LONG_INTEGER:
            {
                int i;for(i = 0;i < vec_ln;++i)
                {
                    ((int*)destination)[i] = ((long long*)object->value)[i];
                }
            }break;
        case GCDL_VECTOR_INTEGER:
            {
                int i;for(i = 0;i < vec_ln;++i)
                {
                    ((int*)destination)[i] = ((int*)object->value)[i];
                }
            }break;
        case GCDL_VECTOR_SHORT_INTEGER:
            {
                int i;for(i = 0;i < vec_ln;++i)
                {
                    ((int*)destination)[i] = ((short*)object->value)[i];
                }
            }break;
        case GCDL_VECTOR_BYTE_INTEGER:
            {
                int i;for(i = 0;i < vec_ln;++i)
                {
                    ((int*)destination)[i] = ((char*)object->value)[i];
                }
            }break;

        case GCDL_VECTOR_DOUBLE:
            {
                int i;for(i = 0;i < vec_ln;++i)
                {
                    ((int*)destination)[i] = ((double*)object->value)[i];
                }
            }break;
        case GCDL_VECTOR_FLOAT:
            {
                int i;for(i = 0;i < vec_ln;++i)
                {
                    ((int*)destination)[i] = ((float*)object->value)[i];
                }
            }break;
    }
}
