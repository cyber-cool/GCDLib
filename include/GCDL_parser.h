#ifndef GCDL_PARSER_H
#define GCDL_PARSER_H

#include <stdio.h>
#include <GCDL_structures.h>

#include <UString.h>

/**
*   Opens a file and reads all it's contents to an array of GCDL objects.
*
*   Return value is a GCDL document with all the objects and file string
*   stored in it.
*/
GCDL_file GCDL_parseFileStream(FILE *f_stream, const char *filename);
/**
*   Reads all the string to an array of GCDL objects.
*
*   Return value is a GCDL document with all the objects and file string
*   stored in it..
*/
GCDL_file GCDL_parseString(uchar *str, const char *filename);

#endif
