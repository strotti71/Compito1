#include "fileHandler.h"

/*
LIBRERIA DI GESTIONE DEI FILE
*/
int apriFile(char *filename, FILE* f)
{
    f = fopen(filename, "r");
    if (f != NULL)
    {
        return 1;
    }
}