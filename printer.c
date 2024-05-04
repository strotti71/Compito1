#include "printer.h"
#include <stdio.h>

void stampaArrayParole(type_parola *arr, int len, char *s)
{
    printf("\n%s (%d elementi):\n", s, len);
    for (int i = 0; i < len; i++)
    {
        printf("%s ", arr[i]);
    }
}

void stampaArrayCaratteri(char *arr, int len, char *s)
{
    printf("\n%s (%d elementi):\n", s, len);
    for (int i = 0; i < len; i++)
    {
        printf("%c", arr[i]);
    }
    fflush(stdout);
}