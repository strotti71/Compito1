#include <string.h>
#include "fileHandler.h"

// #include "stringHandler.h"
int contaCaratteri(FILE *f);

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
    return 0;
}

int contaCaratteri(FILE *f)
{
    // conto i caratteri del file di input per avere una dimensione minima
    // di caratteri da cui iniziare. i caratteri aggiuntivi (dovuti all'inserimento degli spazi)
    // verranno allocati dopo la scansione di ogni parola
    char c;
    int n = 0;
    while ((c = fgetc(f)) != EOF)
    {
        n++;
    }
    rewind(f);
    return n;
}
/*
metodo per la scrittura di un file csv.
il metodo accetta in input
    un array di parole
    il nome file da scrivere

Restituisce -1 se la scrittura del file è fallita*/
int esportaCsv_old(type_parola *source, char *fileName)
{
    int j = 0;
    printf("\n\n STAMPO DA FUNZIONE Esporta...\n\n");
    while (strcmp(source[j], "~") != 0)
    {
        printf("%s ", source[j]);
        fflush(stdout);
        j++;
    }

    FILE *f = fopen(fileName, "w");
    if (f == NULL)
    {
        return -1;
    }
    printf("\n\n");
    j = 0;
    while (strcmp(source[j], "~") != 0)
    {
        fprintf(f, "%s;\n", source[j]);
        fflush(stdout);
        j++;
    }
    return 0;
}

char *getParolaFromnumber(int number)
{
    return "ciao";
}
/*
int esportaCsv(Record *source, int len, char *fileName)
{
    // int j = 0;
    int index = 0;
    printf("\n\n STAMPO DA FUNZIONE Esporta...\n\n");
    while (index < len)
    {
        printf("%d", source[index]);
        fflush(stdout);
        index++;
    }

    FILE *f = fopen(fileName, "w");
    if (f == NULL)
    {
        return -1;
    }
    printf("\n\n");
    index = 0;
    while (index < len)
    {
        printf("%d", source[index]);
        fflush(stdout);
        index++;
    }
    return 0;
}
*/
