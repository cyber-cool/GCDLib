#include "UString.h"

#include <stdlib.h>
#include <string.h>

#define min(x,y)(((x) < (y)) ? (x) : (y))

uchar *str_to_ustr(const char *str)
{
    uchar *new_str = malloc(sizeof(uchar) * (strlen(str)+1));
    uchar *new_str_iter = new_str;
    for(;*str != '\0';++str, ++new_str_iter)
    {
        *new_str_iter = ((uchar)0) | *str;
    }
    *new_str_iter = 0;

    return new_str;
}
char *ustr_to_str(const uchar *ustr)
{
    char *new_str = malloc(sizeof(char) * (ustrlen(ustr)+1));
    char *new_str_iter = new_str;
    for(;*ustr != 0;++ustr, ++new_str_iter)
    {
        *new_str_iter = min(*ustr, 255);
    }
    *new_str_iter = '\0';

    return new_str;
}

int ustrlen(const uchar *str)
{
    int ln;
    for(ln = 0;*str != 0;++str, ++ln){}
    return ln;
}

void ustrcpy(uchar *dest, const uchar *src)
{
    memcpy(dest, src, (ustrlen(src)+1) * sizeof(uchar));
}
uchar *ustrcpy_alloc(const uchar *src)
{
    uchar *new_str = malloc((ustrlen(src)+1) * sizeof(uchar));
    ustrcpy(new_str, src);
    return new_str;
}

void ustrcat(uchar *dest, const uchar *src)
{
    uchar *dest_end = dest + ustrlen(dest);
    memcpy(dest_end, src, (ustrlen(src)) * sizeof(uchar));
}
void ustrcat_alloc(uchar **dest, const uchar *src)
{
    *dest = realloc(*dest, (ustrlen(*dest) + ustrlen(src)) * sizeof(uchar));
    ustrcat(*dest, src);
}

int ustrcmp(const uchar *str1, const uchar *str2)
{
    if(ustrlen(str1) != ustrlen(str2))
    {
        return ustrlen(str1) - ustrlen(str2) > 0 ? 1 : -1;
    }

    for(;*str1 != 0;++str1, ++str2)
    {
        if(*str1 != *str2)
            return -1;
    }
    return 0;
}

uchar *ustrchr(uchar *str, uchar ch)
{
    for(;*str != '\0';++str)
    {
        if(*str == ch)
        {
            return str;
        }
    }
    return NULL;
}
