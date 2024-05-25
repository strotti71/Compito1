#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "occorrenza.h"

/// @brief   metodo per la ricerca di una parola nell'array.w
///          se la parola è presente ne restituisce la posizione.
///          se la parola non è presente restituisce -1
/// @param a                l'array in cui cercare la parola
/// @param parolaCercata    la parola da cercare
/// @param lunghezzaArray   le dimensioni dell'array
/// @return                 l'indice della parola nell'array
int cercaParola(type_parola_w *a, type_parola_w parolaCercata, int lunghezzaArray)
{

    for (int i = 0; i < lunghezzaArray; i++)
    {
        if (wcscmp(parolaCercata, a[i]) == 0)
            return i;
    }
    return -1;
}

/// @brief funzione per convertire il numero occorrenze in una percentuale
/// @param n    il numero in input
/// @param nTot il totale su cui calcolare la percentuale
/// @return     la percentuale restituita
double calcolaOccorrenze(int n, int nTot)
{
    return ((double)n / nTot);
}

/// @brief La funzione restituisce una parola in un array di parole
/// @param arrayParole  l'array da cui prendere la parola
/// @param index        la posizione della parola nell'array
/// @return             la parola
wchar_t *getParola(type_parola_w *arrayParole, int index)
{
    return (arrayParole[index]);
}

/// @brief metodo per l'inserimento di una parola in un array di parole
/// @param a    l'array in cui inserire la parola
/// @param p    la parola da inserire
/// @param lunghezzaArray la posizione in cui inserire la parola
void inserisciParola(type_parola_w *a, type_parola_w p, int lunghezzaArray)
{
    wcscpy(a[(lunghezzaArray)], p);
}

/// @brief funzione che cerca e restituisce l'indice nell'array probability di una parola
/// @param probRec          L'array di probability in cui cercare
/// @param parolaCercata    La parola da cercare
/// @param len              Le dimensioni dell'array
/// @return                 L'indice della parola; -1 se la parola non è presente
int cercaParolaArrayProbability(ProbabilityRecord *probRec, type_parola_w parolaCercata, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (wcscmp(parolaCercata, probRec[i].parola) == 0)
            return i;
    }
    return -1;
}

/// @brief funzione che genera e restituisce un numero casuale compreso tra 0 e 1
/// @return il numero generato
double generateRandomNum()
{
    time_t timer;
    srand((unsigned)time(&timer));
    return ((double)rand() / RAND_MAX);
}

/// @brief funzione che converte un wChar_t* in double: utilizzata per calcolare le probabilità lette dal file csv
/// @param cWord la parola da convertire
/// @return      un double
double wCharToDouble(wchar_t cWord[_MAX_LENGTH_WORD_])
{
    char mbstr[100];                       // creo array  abbastanza grande per contenere la stringa multibyte
    wcstombs(mbstr, cWord, sizeof(mbstr)); // Converto la stringa wide character in una stringa multibyte

    double result = strtod(mbstr, NULL); // Converto la stringa multibyte in double
    return result;                       // Restituisco il risultato come double
}

/// @brief     funzione per la generazione di un numero casuale (tra 0 e 1) utilizzando il timer di sistema
/// @return    il numero casuale generato
unsigned long getMilliseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long)(tv.tv_sec) * 1000 + (unsigned long)(tv.tv_usec) / 1000;
}