#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "occorrenza.h"

/*
Metodo per la creazione di un nuovo record.
*/
Record *inserisciRecord(char parola[_MAX_LENGTH_WORD_], type_parola parolaSuccessiva)
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
int cercaParola(type_parola *a, type_parola parolaCercata, int lunghezzaArray)
{

    for (int i = 0; i < lunghezzaArray; i++)
    {
        if (strcmp(parolaCercata, a[i]) == 0)
            return i;
    }
    return -1;
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
void inserisciParola(type_parola *a, type_parola p, int lunghezzaArray)
{
    strcpy(a[(lunghezzaArray)], p);
}
