#include "printer.h"
#include <stdio.h>

void stampaArray(type_parola *arr, int len)
{
    printf("\nStampo array (%d elementi):\n", ++len);
    for (int i = 0; i < len; i++)
    {
        printf("%s ", arr[i]);
    }
}