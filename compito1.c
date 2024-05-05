#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "fileHandler.h"
#include "stringHandler.h"
#include "occorrenza.h"
#include "printer.h"
/*
#include "fileHandler.c"
#include "stringHandler.c"
#include "occorrenza.c"
#include "printer.c"


1) Apro il file di input
2) copio il contenuto del file in  array(char) fileNormalizzato che ha le seguenti proprietà
    - sono stati rimossi tutti i doppi spazi
    - la punteggiatura viene separata dalle parole precedenti e seguenti tramite uno spazio
3) chiudo il file di input
4) creo arrayParole che contiene le singole istanze di tutte le parole

*/
int nCharInput; // numero caratteri nel file di input

type_parola arrayParole[10000];    // Array contiene tutte le singole istanze delle parole
int numeroDistinctParoleTesto = 0; // numero di parole distinte nel testo. ciascuna parola viene caricata nell'array parole
int numeroParoleTotali = 0;

char *fileNormalizzato;    // file riportato su array con eliminazione dei doppi spazi e separazione di tutti i caratteri di punteggiatura
int nCharFileNormalizzato; // numero caratteri del file normalizzato

char stringone[10000]; // array in cui viene
Record *arrayRecordParole;

void popolaArrayParole(char *fin);
void popolaArrayRecordOccorrenze(char *fin);
char *preparaStream(FILE *fin);
int contaCaratteri(FILE *f);
type_parola *getParola(type_parola *arrayPaole, int index);

// int parolaRegistrata(char parola[], Record *record, int numeroParoleTrovate);
// void stampaStatistiche(Record *arrayRecord, int numeroParoleTrovate);

char *getNomeFile(int args, char *argv[]); // funzione che restituisce il nome file da argomento di avvio
void leggiFile(FILE *fin);

char nomeFile[] = "ciao.txt"; // file di testo di default

int main(int args, char *argv[])
{
    FILE *fin;
    /**
     * APERTURA DEL FILE
     */
    strcpy(nomeFile, getNomeFile(args, argv));
    printf("\033[3J");
    fin = fopen(nomeFile, "r");
    if (fin == NULL)
    {
        printf("\n\nImpossibile aprire il file %s.\nEsco\n", nomeFile);
        return 1;
    }
    // nCharInput = contaCaratteri(fin);
    // rewind(fin);
    printf("Numero caratteri nel testo: %d", nCharInput);

    // inizio la lettura del file:
    // la funzione crea un *char filenormalizzato in cui
    // tutte le parole e i segni di interpunzione sono separati da uno spazio
    leggiFile(fin);

    // chiudo il file
    if (fin != NULL)
        fclose(fin);

    // la funzione prende il file normalizzato e scrive arrayParole:
    // un array di parole distinte presenti nel testo
    popolaArrayParole(fileNormalizzato);

    stampaArrayParole(arrayParole, numeroDistinctParoleTesto, "\nParole distinte del testo: ");
    printf("\nNumero Parole distinte: %d\nNumero Parole Totali: %d\n", numeroDistinctParoleTesto, numeroParoleTotali);

    // creo arrayRecordParole dimensionato con il numero di parole distinte presenti nel testo
    arrayRecordParole = calloc(numeroDistinctParoleTesto, sizeof(Record));
    arrayRecordParole->numeroParoleSuccessive = 0;

    // costruisco l'array di record che contengono i conteggi delle parole successive
    popolaArrayRecordOccorrenze(fileNormalizzato);

    // esportaCsv(arrayRecordParole, numeroParoleTotali, "export.csv");
    // esportaCsv(arrayRecordParole, numeroParoleTotali, "export.csv");

    int index = 0;

    while (index < numeroParoleTotali)
    {
        printf("\n%s: ", getParola(arrayParole, index));
        for (int j = 0; j < arrayRecordParole[index].numeroParoleSuccessive; j++)
        {
            printf("segue: %s (%d))", getParola(arrayParole, arrayRecordParole[index].occorrenze[j].parolaSuccessiva), arrayRecordParole[index].occorrenze[j].numeroOccorrenze);
            fflush(stdout);
        }

        index++;
    }

    FILE *f = fopen("export.csv", "w");
    if (f == NULL)
    {
        return -1;
    }
    printf("\n\n");
    index = 0;
    while (index < numeroParoleTotali)
    {
        //    printf("%d", arrayRecordParole[index]);
        fflush(stdout);
        index++;
    }
}

/*
La funzione restituisce il nome file
    se è specificato il primo argomento in argv[] allora  l'argomento è il nome del file
    altrimenti viene restituito un file di default
*/
char *getNomeFile(int args, char *argv[])
{
    return (args > 1 ? argv[1] : nomeFile);
}

/*
    CICLO DI LETTURA DELLE RIGHE DEL FILE

    ParolaPrecedente = . (la prima parola non investigata è .
    ParolaSuccessiva è la parola che viene letta.
    terminata la parola succcessiva
        viene registrata parola precedente
        viene registrata parola successiva
        parolaprecedente diventa parolasuccessiva
    Se incontro i seguenti caratteri : _ \n la parola è terminata
    . , ! ?
*/
void leggiFile(FILE *fin)
{

    fileNormalizzato = preparaStream(fin);
    // stampaArrayCaratteri(fileNormalizzato, nCharFileNormalizzato, "file normalizzato: ");

    /*
      lettura dell'array pre-processato:
      carico tutte le parole distinte in un array arrayParole[]
      es: arrayParole{. quel ramo del lago di como , ...}
      */
}

/*
il metodo legge il file e copia tutte le parole nell'array stringone eseguendo le seguenti correzioni:
1- inserisce spazi prima e dopo la punteggiatura
2- elimina doppi spazi
al termine dell'elaborazione ciascuna parola o segno di punteggiatura sono separati da uno spazio
*/
char *preparaStream(FILE *fin)
{
    // TO DO 2: messo temporaneamente ~ in coda all'array
    char c;
    char cPrec = ' ';
    nCharFileNormalizzato = 0;

    while ((c = fgetc(fin)) != EOF)
    {
        if ((c < 32) || (c > 126))
            printf("Trovato carattere sporco%c", c);
        if (isPunteggiatura(c))
        {
            if (cPrec != ' ')
            {
                appendCharToString(stringone, ' ', nCharFileNormalizzato);
                nCharFileNormalizzato++;
                cPrec = ' ';
            }
            appendCharToString(stringone, c, nCharFileNormalizzato);
            nCharFileNormalizzato++;
            appendCharToString(stringone, ' ', nCharFileNormalizzato);
            nCharFileNormalizzato++;
            cPrec = ' ';
        }
        else
        {
            // se il carattere precedente e il carattere letto sono entrambi spazi,
            // non inserisco il carattere per evetiare doppi spazi inutili
            c = tolower(c);
            if (!((cPrec == c) && (c == 32)))
                appendCharToString(stringone, c, nCharFileNormalizzato);
            nCharFileNormalizzato++;
            cPrec = c;
        }
    }
    return stringone;

    // return ". Quel ramo del lago di Como , che lago volge lago , , a mezzogiorno . ";
}

/* seconda lettura del file: popolo l'array di record
per ciascuna parola che leggo nel file:
    - cerco l'indice sull'arrayParole
       (es: quel = 1)
       cerco l'indice in record[].parola
           - se non è presente inserisco l'indice in coda all'array
    da terminare per le parole successive e le occorrenze
*/

void popolaArrayRecordOccorrenze(char *testo)
{
    char c;
    int indexChar = 0;
    int indexTesto = 0;

    int indicePrecedente = -1;
    int indiceSuccessiva;

    int indiceParolaInArrayStructParole = 0;
    type_parola parolaPrecedente = "."; // l'ultima parola letta
    type_parola parolaSuccessiva;       // la parola che sto leggendo

    pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

    while (indexTesto < nCharFileNormalizzato)
    {
        c = testo[indexTesto];
        if (isSeparator(c))
        {
            // chiudo la parola che sto costruendo
            appendCharToString(parolaSuccessiva, '\0', indexChar);

            // cerco l'indice della parola appena letta nella'rray parole
            indiceSuccessiva = cercaParola(arrayParole, parolaSuccessiva, numeroDistinctParoleTesto);

            // cerco l'indice della parola precedente
            indicePrecedente = cercaParola(arrayParole, parolaPrecedente, numeroDistinctParoleTesto);
            // creo un nuovo record copia del record corrispondente alla parola precedente
            Record RecordParolaTemp = arrayRecordParole[indicePrecedente];

            int trovato = 0;
            for (int i = 0; (i < RecordParolaTemp.numeroParoleSuccessive && !trovato); i++)
            {
                if (RecordParolaTemp.occorrenze[i].parolaSuccessiva == indiceSuccessiva)
                {
                    printf("trovato occorrenza %d\n", RecordParolaTemp.occorrenze[i].parolaSuccessiva);
                    RecordParolaTemp.occorrenze[i].numeroOccorrenze = RecordParolaTemp.occorrenze[i].numeroOccorrenze + 1; //= arrayRecordParole->occorrenze[i].numeroOccorrenze + 1;
                    RecordParolaTemp.numeroParoleSuccessive = RecordParolaTemp.numeroParoleSuccessive + 1;                 //= arrayRecordParole->numeroParoleSuccessive + 1;
                    trovato = 1;
                    arrayRecordParole[indicePrecedente] = RecordParolaTemp;
                }
            }
            // non ho trovato occorrenza successiva:
            if (!trovato)
            {
                // 1 creo array più grande
                int newCountOccorrenze = RecordParolaTemp.numeroParoleSuccessive + 1;

                // rialloco la memoria per arrayTemp.occorrenze[];
                // RecordParolaTemp.occorrenze = realloc(RecordParolaTemp.occorrenze, newCountOccorrenze * sizeof(Occorrenza));
                // printf("\n%s %s %d\n", parolaPrecedente, parolaSuccessiva, RecordParolaTemp.occorrenze[newCountOccorrenze].numeroOccorrenze);
                // fflush(stdout);

                RecordParolaTemp.occorrenze[newCountOccorrenze - 1].parolaSuccessiva = indiceSuccessiva;
                RecordParolaTemp.occorrenze[newCountOccorrenze - 1].numeroOccorrenze = 1;
                RecordParolaTemp.numeroParoleSuccessive++;

                /*                 Occorrenza *newOcc = (Occorrenza *)calloc(newCountOccorrenze, sizeof(Occorrenza));
                             for (int i = 0; i < RecordParolaTemp.numeroParoleSuccessive; i++)

                             {
                                 newOcc[i] = RecordParolaTemp.occorrenze[i];
                             }

             newOcc[newCountOccorrenze].parolaSuccessiva = indiceSuccessiva;
             newOcc[newCountOccorrenze].numeroOccorrenze = 1;

             free(RecordParolaTemp.occorrenze);
*/

                // RecordParolaTemp.occorrenze = newOcc;
                //   RecordParolaTemp.numeroParoleSuccessive++;
                // arrayRecordParole[indicePrecedente] = RecordParolaTemp;
                //  printf("\nPrecedente:'%s'\tSuccessiva:'%s':\t num parole successive:%d", parolaPrecedente, parolaSuccessiva, arrayRecordParole->numeroParoleSuccessive);

                indiceParolaInArrayStructParole++;

                arrayRecordParole[indicePrecedente] = RecordParolaTemp;
                // arrayRecordParole[indicePrecedente].occorrenze = realloc(arrayRecordParole[indicePrecedente].occorrenze, newCountOccorrenze * sizeof(Occorrenza));
            }
            // rialloco lo spazio per il record indice precedente

            /*  printf("\n%d\n", arrayRecordParole[indicePrecedente].occorrenze[1].numeroOccorrenze);
              fflush(stdout);
  */
            pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
            strcpy(parolaPrecedente, parolaSuccessiva);
            pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

            indexTesto++;
            indexChar = 0;
        }
        else
        {
            if (indexChar < _MAX_LENGTH_WORD_)
            {
                parolaSuccessiva[indexChar] = c;
                indexChar++;
                indexTesto++;
            }
        }
    }
}

/*
Metodo per la prima lettura del file.
il metodo legge il file fino alla fine e scrive tutte le parole trovate in un distinctArray
*/

void popolaArrayParole(char *fin)
{
    char c;
    int indexChar = 0;
    int indexFin = 0;
    type_parola parola = ".";

    inserisciParola(arrayParole, parola, numeroDistinctParoleTesto);
    numeroDistinctParoleTesto++;

    for (int i = 0; i < nCharFileNormalizzato; i++)
    {
        c = (fin[indexFin]);
        if (!((c < 32) || (c > 126)))
        {
            if (isSeparator(c) || isPunteggiatura(c))
            {
                /*
                se ho trovato un separatore ho una nuova parola e la inserisco nell'array
                */
                appendCharToString(parola, '\0', indexChar);
                numeroParoleTotali++;

                if (cercaParola(arrayParole, parola, numeroDistinctParoleTesto) < 0)
                {
                    inserisciParola(arrayParole, parola, numeroDistinctParoleTesto);
                    numeroDistinctParoleTesto++;
                }
                pulisciStringa(parola, _MAX_LENGTH_WORD_);
                indexChar = 0;
            }
            else
            {
                if (indexChar < _MAX_LENGTH_WORD_)
                {
                    parola[indexChar] = c;
                    indexChar++;
                }
            }
        }
        indexFin++;
    }
}
int contaCaratteri(FILE *f)
{
    /*
    Il metodo restituisce il numero di caratteri nel file di testo in input
    */
    char c;
    int n = 0;
    while ((c = fgetc(f)) != EOF)
    {
        n++;
    }
    return n;
}
void stampaStatistiche(Record *arrayRecord, int numeroParoleTrovate)
{

    printf("\n\n\n**********************************************");
    printf("\n***********STAMPO LE STATISTICHE***************\n");
    printf("\n\nNumero Parole distinte: %d\n\nNumero Parole Totali: %d\n\n", numeroDistinctParoleTesto, numeroParoleTotali);
    fflush(stdout);
    //  sleep(1);
    // }
    printf("\n\n");
    for (int i = 0; i < 100; i++)
        printf("%s ", arrayParole[i]);
    printf("\n\n\n******FINE STAMPO LE STATISTICHE***************");
    printf("\n**********************************************\n\n\n");
}
type_parola *getParola(type_parola *arrayParole, int index)
{
    return (&arrayParole[index]);
}
