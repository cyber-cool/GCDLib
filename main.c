#include <stdio.h>
#include <stdlib.h>
#include "GCDL_parser.h"
#include "GCDL_iterator.h"

void recursive_print(GCDL_object *obj_arr, size_t obj_arr_sz, int depth)
{
    size_t i;for(i = 0;i < obj_arr_sz;++i)
    {
        int d;for(d = 0;d < depth;++d)
            printf("\t");

        printf("%S :", obj_arr[i].name);
        if(obj_arr[i].value)
        {
            switch(obj_arr[i].value_type)
            {
                case GCDL_VALUE_STRING:
                    printf("[ %S ]" ,(uchar*)obj_arr[i].value);
                    break;
                case GCDL_VALUE_INTEGER:
                    printf("< %d >", *((int*)obj_arr[i].value));
                    break;
                case GCDL_VALUE_FLOAT:
                    printf("< %f >", *((float*)obj_arr[i].value));
                    break;
                case GCDL_VALUE_DOUBLE:
                    printf("< %f >", *((double*)obj_arr[i].value));
                    break;

                case GCDL_VECTOR_INTEGER:
                    {
                        printf("< ");
                        int j;for(j = 0;j < -obj_arr[i].children_count;++j)
                        {
                            printf("%d,", ((int*)obj_arr[i].value)[j]);
                        }
                        printf(" >");
                    } break;
                case GCDL_VECTOR_DOUBLE:
                    {
                        printf("< ");
                        int j;for(j = 0;j < -obj_arr[i].children_count;++j)
                        {
                            printf("%f,", ((double*)obj_arr[i].value)[j]);
                        }
                        printf(" >");
                    } break;
            }
        }
        printf("\n");

        if(obj_arr[i].children && obj_arr[i].children_count > 0)
            recursive_print(obj_arr[i].children, obj_arr[i].children_count, depth + 1);
    }
}
void print_errors(GCDL_error *errors, size_t errors_cnt)
{
    printf("printing %d errors\n", errors_cnt);
    size_t i;for(i = 0;i < errors_cnt;++i)
    {
        printf("[%d] %s\n", errors[i].ID, errors[i].description);
        printf("line %ld:\n", errors[i].line);
        printf("////////////////////////////////////////\n");
        printf("%.32S\n", errors[i].str_ptr);
        printf("////////////////////////////////////////\n");
    }
}

int main()
{
    FILE *gcd_f = fopen("example.gcdl", "r");
    GCDL_file doc = GCDL_parseFileStream(gcd_f, "example.gcdl");

    //recursive_print(doc.objects, doc.objects_cnt, 0);
    //print_errors(doc.errors, doc.error_cnt);

    char _void;
    scanf("%c\n", &_void);
    return 0;
}
