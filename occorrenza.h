#include "stringHandler.h"

int cercaParola(type_parola *a, type_parola parolaCercata, int lunghezzaArray);

void inserisciParola(type_parola *a, type_parola p, int lunghezzaArray);

/*
struttura per registrare un'occorrenza di parola successiva.
 - parolaSuccessiva è un intero corrispondente alla cella dell'arrayParole
 - percentuale: double la percentuale di occorrenze per quella parola
*/
typedef struct
{
    int parolaSuccessiva;
    int numeroOccorrenze;
} Occorrenza;

/*
Struttra per registrare i dati di ciascuna parola.

    - La parola è un intero corrispondente alla cella dell'arrayParole
    - il numero di parole successive trovate
    - un array di occorrenze delle parole trovate. (inizializzato a 0)
        ciascun occorrenza contiene :
            - la parola
            - la statistica di tale parola trovata nel testo
*/
typedef struct
{
    Occorrenza *occorrenze;
    int numeroParoleSuccessive;
} Record;

// int cercaIntero(Record *rec, int numeroCercato, int lunghezzaArray);