#ifndef GCDL_ITERATOR_H
#define GCDL_ITERATOR_H

#include "GCDL_structures.h"

typedef void(*GCDL_foreachObject)(GCDL_object *object);

/**
*       Iterates over GCDL file, calling a function corresponding
*       to every GCDL_object type.
*
*       Specify depth == -1 to iterate all the objects.
*/
void GCDL_iterate_over_file(GCDL_file *file,
                       int depth,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector);
void GCDL_iterate_over_file_recursively(GCDL_file *file,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector);
void GCDL_iterate_over_file_root(GCDL_file *file,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector);

/**
*       Iterates over an array of GCDL objects, calling
*       a function corresponding to every GCDL_object type.
*
*       Specify depth == -1 to iterate all the objects.
*/
void GCDL_iterate_over_array(GCDL_object *array, size_t array_size,
                       int depth,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector);
void GCDL_iterate_over_array_recursively(GCDL_object *array, size_t array_size,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector);
void GCDL_iterate_over_array_root(GCDL_object *array, size_t array_size,
                       GCDL_foreachObject foreach_structure,
                       GCDL_foreachObject foreach_field,
                       GCDL_foreachObject foreach_array,
                       GCDL_foreachObject foreach_arrayItem,
                       GCDL_foreachObject foreach_vector);

// Some useful parsing tools

void GCDL_assign_object_valuel(GCDL_object *object, long long *destination);
void GCDL_assign_object_valuei(GCDL_object *object, int *destination);
void GCDL_assign_object_values(GCDL_object *object, short *destination);
void GCDL_assign_object_valueb(GCDL_object *object, unsigned char *destination);
void GCDL_assign_object_valued(GCDL_object *object, double *destination);
void GCDL_assign_object_valuef(GCDL_object *object, float *destination);

/** Destination is interpreted as an array that has required capacity. **/
void GCDL_assign_object_vectori(GCDL_object *object, int *destination);

#endif
