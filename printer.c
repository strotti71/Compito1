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

void stampaParametri(ParametriInput *parametri)
{
    printf("\n*******PARAMETRI INPUT**********");
    printf("\n********************************");
    printf("\nFile In Input: %s", parametri->inputFileName);
    printf("\nNumero parole di testo da generare: %d", parametri->nParoleDaGenerare);
    printf("\nPrima parola del testo da generare: %s", parametri->startingWord);

    printf("\n********************************");
    printf("\n********************************");
}