#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameterHandler.h"
#include "stringHandler.h"

char defaultFile[50] = "tempo.txt";

//***********************************************************************
//                                                                      *
//                    LIBRERIA PER LA GESTIONE DEI PARAMETRI            *
//                                                                      *
//***********************************************************************

/// @brief funzione per la gestione dei parametri in input:
//  -h  stampa il manuale del programma
//  -i  file da leggere in input
//  -n  numero di parole che il programma deve generare
//  -s  prima parola del testo da generare
// inizializza i parametri con i valori di default
// utilizzati se non c'è passaggio del parametro.
/// @param argc numero di argomenti passati in input al main del programma
/// @param argv array di argomenti
/// @param parametri una struct in cui memorizzare le informazioni dei parametri lette
void leggiParametri(int argc, char *argv[], ParametriInput *parametri)
{
    parametri->help = 0;
    parametri->nParoleDaGenerare = 100;
    strcpy(parametri->inputFileName, "tempo.txt");
    strcpy(parametri->startingWord, ".");

    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
        parametri->help = 1;
    else
    {
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
}
