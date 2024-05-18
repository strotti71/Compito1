#include <stdio.h>
#include "printer.h"

/// @brief funzione generalizzata per la stampa di un array di parole;
/// @param arr L'array di parole che si vuole stampare
/// @param len il numero di parole
/// @param s   la stringa descrittiva per l'intestazione della stampa
void stampaArrayParole(type_parola *arr, int len, char *s)
{
    printf("\n%s (%d elementi):\n", s, len);
    for (int i = 0; i < len; i++)
    {
        printf("%s ", arr[i]);
    }
} /// @brief funzione generalizzata per la stampa di un array di caratteri;
/// @param arr L'array di caratteri che si vuole stampare
/// @param len il numero di caratteri da stampare
/// @param s   l'intestazione da stampare prima dell'elenco delle righe
void stampaArrayCaratteri(char *arr, int len, char *s)
{
    printf("\n%s (%d elementi):\n", s, len);
    for (int i = 0; i < len; i++)
    {
        printf("%c", arr[i]);
    }
    fflush(stdout);
}

/// @brief funzione per la stampa decorata dei parametri in input
/// inserti dall'utente
/// @param parametri la struttura (ParametriInput) contenente i parametri inseriti
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
