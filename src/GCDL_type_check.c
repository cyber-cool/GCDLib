#include "GCDL_type_check.h"
#include "GCDL_structures.h"

int GCDL_isStructure(const GCDL_object *object)
{
    return object->type == GCDL_OBJECT_STRUCTURE;
}
int GCDL_isField(const GCDL_object *object)
{
    return object->type == GCDL_OBJECT_FIELD;
}
int GCDL_isArray(const GCDL_object *object)
{
    return object->type == GCDL_OBJECT_ARRAY;
}
int GCDL_isArrayItem(const GCDL_object *object)
{
    return object->type == GCDL_OBJECT_ARRAY_ITEM;
}
int GCDL_isVector(const GCDL_object *object)
{
    return object->type == GCDL_OBJECT_VECTOR;
}

int GCDL_isStringValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_STRING;
}
int GCDL_isCharacterValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_CHARACTER;
}
int GCDL_isIntegerValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_INTEGER;
}
int GCDL_isShortIntegerValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_SHORT_INTEGER;
}
int GCDL_isByteIntegerValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_BYTE_INTEGER;
}
int GCDL_isLongIntegerValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_LONG_INTEGER;
}
int GCDL_isAnyIntegerValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_BYTE_INTEGER
        || object->value_type == GCDL_VALUE_SHORT_INTEGER
        || object->value_type == GCDL_VALUE_INTEGER
        || object->value_type == GCDL_VALUE_LONG_INTEGER;
}
int GCDL_isFloatValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_FLOAT;
}
int GCDL_isDoubleValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_DOUBLE;
}
int GCDL_isAnyRealValue(const GCDL_object *object)
{
    return object->value_type == GCDL_VALUE_DOUBLE
        || object->value_type == GCDL_VALUE_FLOAT;
}

int GCDL_isStringVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_STRING;
}
int GCDL_isCharacterVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_CHARACTER;
}
int GCDL_isIntegerVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_INTEGER;
}
int GCDL_isShortIntegerVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_SHORT_INTEGER;
}
int GCDL_isByteIntegerVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_BYTE_INTEGER;
}
int GCDL_isLongIntegerVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_LONG_INTEGER;
}
int GCDL_isAnyIntegerVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_BYTE_INTEGER
        || object->value_type == GCDL_VECTOR_SHORT_INTEGER
        || object->value_type == GCDL_VECTOR_INTEGER
        || object->value_type == GCDL_VECTOR_LONG_INTEGER;
}
int GCDL_isFloatVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_FLOAT;
}
int GCDL_isDoubleVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_DOUBLE;
}
int GCDL_isAnyRealVector(const GCDL_object *object)
{
    return object->value_type == GCDL_VECTOR_DOUBLE
        || object->value_type == GCDL_VECTOR_FLOAT;
}
