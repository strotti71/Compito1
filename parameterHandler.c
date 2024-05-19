#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameterHandler.h"
#include "stringHandler.h"

char defaultFile[30] = "tempo.txt";
void leggiParametri(int argc, char *argv[], ParametriInput *parametri)
{
    // funzione per la gestione dei parametri in input:
    //  -i  file da leggere in input
    //  -n  numero di parole che il programma deve generare
    //  -s  prima parola del testo da generare

    // inizializza i parametri con i valori di default
    // utilizzati se non c'è passaggio del parametro.
    parametri->nParoleDaGenerare = 100;
    strcpy(parametri->inputFileName, "tempo.txt");
    strcpy(parametri->startingWord, ".");

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            // parametro -i = input di un file
            if (i + 1 < argc)
            {
                pulisciStringaChar(parametri->inputFileName, 30);
                strcpy(parametri->inputFileName, argv[i + 1]);
                i++;
            }
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            // parametro -n: numero di parole da generare
            if (i + 1 < argc)
            {
                parametri->nParoleDaGenerare = atoi(argv[i + 1]);
                i++;
            }
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            // parametro -s: parola iniziale nel testo generato
            if (i + 1 < argc)
            {
                pulisciStringaChar(parametri->startingWord, 30);
                strcpy(parametri->startingWord, argv[i + 1]);
                i++;
            }
        }
    }
}
