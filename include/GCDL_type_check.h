#ifndef GCDL_UTILITIES_H
#define GCDL_UTILITIES_H

#include "GCDL_structures.h"

/** Generic GCDL types **/
int GCDL_isStructure(const GCDL_object *object);
int GCDL_isField(const GCDL_object *object);
int GCDL_isArray(const GCDL_object *object);
int GCDL_isArrayItem(const GCDL_object *object);
int GCDL_isVector(const GCDL_object *object);

/** Simple types **/
int GCDL_isStringValue(const GCDL_object *object);
int GCDL_isCharacterValue(const GCDL_object *object);
int GCDL_isIntegerValue(const GCDL_object *object);
int GCDL_isShortIntegerValue(const GCDL_object *object);
int GCDL_isByteIntegerValue(const GCDL_object *object);
int GCDL_isLongIntegerValue(const GCDL_object *object);
int GCDL_isAnyIntegerValue(const GCDL_object *object);

int GCDL_isFloatValue(const GCDL_object *object);
int GCDL_isDoubleValue(const GCDL_object *object);
int GCDL_isAnyRealValue(const GCDL_object *object);

/** Vectors of simple types **/

int GCDL_isStringVector(const GCDL_object *object);
int GCDL_isCharacterVector(const GCDL_object *object);
int GCDL_isIntegerVector(const GCDL_object *object);
int GCDL_isShortIntegerVector(const GCDL_object *object);
int GCDL_isByteIntegerVector(const GCDL_object *object);
int GCDL_isLongIntegerVector(const GCDL_object *object);
int GCDL_isAnyIntegerVector(const GCDL_object *object);

int GCDL_isFloatVector(const GCDL_object *object);
int GCDL_isDoubleVector(const GCDL_object *object);

#endif
