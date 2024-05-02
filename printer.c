#include "printer.h"
#include <stdio.h>

void stampaArray(type_parola *arr, int len, char *s)
{
    printf("\n%s (%d elementi):\n", s, len);
    for (int i = 0; i < len; i++)
    {
        printf("%s ", arr[i]);
    }
}