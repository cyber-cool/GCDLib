#ifndef USTRING_H
#define USTRING_H

#include <stdlib.h>

typedef short uchar;

/**
*       Converts usual string (array of char) into an array of uchar.
*/
uchar *str_to_ustr(const char *str);
/**
*       Converts ustring to normal string. All characters above
*       the value of 255 will be cut.
*/
char *ustr_to_str(const uchar *ustr);

int ustrlen(const uchar *str);

void ustrcpy(uchar *dest, const uchar *src);
uchar *ustrcpy_alloc(const uchar *src);

void ustrcat(uchar *dest, const uchar *src);
void ustrcat_alloc(uchar **dest, const uchar *src);

int ustrcmp(const uchar *str1, const uchar *str2);

uchar *ustrchr(uchar *str, uchar ch);

#endif
