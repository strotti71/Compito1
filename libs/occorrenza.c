#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "occorrenza.h"

/*
Metodo per la creazione di un nuovo record.
*/
Record *inserisciRecord(wchar_t parola[_MAX_LENGTH_WORD_], type_parola_w parolaSuccessiva)
{
    Record *r = (Record *)malloc(sizeof(Record));
    if (r == NULL)
        printf("ERRORE");
    Occorrenza *occ = (Occorrenza *)malloc(sizeof(Occorrenza));
    if (occ == NULL)
        printf("ERRORE");
    // strcpy(r->parola, parola);

    //   strcpy(r->occorrenze[0].parolaSuccessiva, parolaSuccessiva);
    return r;
}

/**
 * metodo per la ricerca di una parola nell'array.
 * se la parola è presente ne restituisce la posizione.
 * se la parola non è presente restituisce -1
 */
int cercaParola(type_parola_w *a, type_parola_w parolaCercata, int lunghezzaArray)
{

    for (int i = 0; i < lunghezzaArray; i++)
    {
        if (wcscmp(parolaCercata, a[i]) == 0)
            return i;
    }
    return -1;
}
double calcolaOccorrenze(int n, int nTot)
{
    return ((double)n / nTot);
}

wchar_t *getParola(type_parola_w *arrayParole, int index)
{
    return (arrayParole[index]);
}
/**
 * metodo per la ricerca di una intero nell'array di record.
 * se il numero è presente ne restituisce la posizione.
 * se il numero non è presente restituisce -1

int cercaIntero(Record *rec, int numeroCercato, int lunghezzaArray)
{
    for (int i = 0; i < lunghezzaArray; i++)
    {
        if (rec[i] == numeroCercato)
            return i;
    }
    return -1;
}
*/

/*
metodo per l'inserimento di una parola nell'array*/
void inserisciParola(type_parola_w *a, type_parola_w p, int lunghezzaArray)
{
    wcscpy(a[(lunghezzaArray)], p);
}

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

double wCharToDouble(wchar_t cWord[_MAX_LENGTH_WORD_])
{
    char mbstr[100];                       // Assicurati che l'array sia abbastanza grande per contenere la stringa multibyte
    wcstombs(mbstr, cWord, sizeof(mbstr)); // Converte la stringa wide character in una stringa multibyte

    double result = strtod(mbstr, NULL); // Converte la stringa multibyte in double
    return result;                       // Restituisce il risultato come double
}