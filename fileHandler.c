#include "fileHandler.h"

/*
LIBRERIA DI GESTIONE DEI FILE
*/
int apriFile(char *filename, FILE *f)
{
    f = fopen(filename, "r");
    if (f != NULL)
    {
        return 1;
    }
}

/*
metodo per la scrittura di un file csv.
il metodo accetta in input
    un array di parole
    il nome file da scrivere

Restituisce -1 se la scrittura del file è fallita*/
int esportaCsv(char *source, char *fileName)
{
    FILE *f = fopen(fileName, "w");
    if (f == NULL)
    {
        return -1;
    }
    fclose(f);
    return 0;
}