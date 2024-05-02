#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "fileHandler.h"
#include "stringHandler.h"
#include "occorrenza.h"
#include "printer.h"
/*
#include "fileHandler.c"
#include "stringHandler.c"
#include "occorrenza.c"
#include "printer.c"

/*
1) Apro il file di input
2) copio il contenuto del file in  array(char) fileNormalizzato che ha le seguenti proprietà
    - sono stati rimossi tutti i doppi spazi
    - la punteggiatura viene separata dalle parole precedenti e seguenti tramite uno spazio
3) chiudo il file di input
4) creo arrayParole che contiene le singole istanze di tutte le parole

*/

type_parola arrayParole[1000000];  // Array contiene tutte le singole istanze delle parole
int numeroDistinctParoleTesto = 0; // numero di parole distinte nel testo. ciascuna parola viene caricata nell'array parole
int numeroParoleTotali = 0;

char *fileNormalizzato;    // file riportato su array con eliminazione dei doppi spazi e separazione di tutti i caratteri di punteggiatura
int nCharFileNormalizzato; // numero caratteri del file normalizzato

char stringone[1000000]; // array in cui viene
Record *arrayRecordParole;

void popolaArrayParole(char *fin);
void popolaArrayRecordOccorrenze(char *fin);
char *preparaStream(FILE *fin);
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
    else
    {
        printf("\n\nIl file...%s è correttamente aperto\n", nomeFile);
    }

    // inizio la lettura del file:
    // la funzione crea un *char filenormalizzato in cui
    // tutte le parole e i segni di interpunzione sono separati da uno spazio
    leggiFile(fin);

    // chiudo il file
    if (fin != NULL)
    {
        printf("\nChiudo il file %s\n", nomeFile);
        fclose(fin);
    }

    // la funzione prende il file normalizzato e scrive arrayParole:
    // un array di parole distinte presenti nel testo
    popolaArrayParole(fileNormalizzato);
    stampaArray(arrayParole, numeroDistinctParoleTesto, "\nParole distinte del testo: ");
    printf("\nNumero Parole distinte: %d\nNumero Parole Totali: %d\n", numeroDistinctParoleTesto, numeroParoleTotali);

    // creo arrayRecordParole dimensionato con il numero di parole distinte presenti nel testo
    arrayRecordParole = calloc(numeroDistinctParoleTesto, sizeof(Record));
    arrayRecordParole->numeroParoleSuccessive = 0;

    for (int i = 0; i < numeroDistinctParoleTesto; i++)
    {
        printf("%d ", arrayRecordParole->numeroParoleSuccessive);
        fflush(stdout);
    }
    printf("\n");
    for (int i = 0; i < nCharFileNormalizzato; i++)
    {
        char ch = (fileNormalizzato[i]);
        printf("%c", ch);
    }

    // costruisco l'array di record che contengono i conteggi delle parole successive
    popolaArrayRecordOccorrenze(fileNormalizzato);

    for (int i = 0; i < 14; i++)
    {
        printf("\n%s  (posizione = %d)  numeroParoleSuccessive: %d", arrayParole[i], i, arrayRecordParole[i].numeroParoleSuccessive);
        for (int j = 0; j < arrayRecordParole[i].numeroParoleSuccessive; j++)
            printf("\n\t Parola successiva: %d", arrayRecordParole[i].occorrenze[j].parolaSuccessiva);
    }

    esportaCsv(arrayParole, "export.csv");
    return (0);
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

    // array di record (ciascun record è una parola trovata)
    Record arrayRecord[100000];

    // dimensione dell'array di record
    int numeroParoleTrovate = 0;
    int contatoreTotale = 0;

    char c;
    int indexChar = 0;
    char parolaPrecedente[_MAX_LENGTH_WORD_] = ".";
    char parolaSuccessiva[_MAX_LENGTH_WORD_] = "";

    printf("\nstarting");
    fileNormalizzato = preparaStream(fin);

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
            if ((cPrec == c) != ' ')
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
    type_parola parolaPrecedente = ".";
    type_parola parolaSuccessiva;

    // pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
    pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

    while (indexTesto < nCharFileNormalizzato)
    {
        c = testo[indexTesto];
        if (isSeparator(c))
        {
            // chiudo la parola che sto costruendo
            appendCharToString(parolaSuccessiva, '\0', indexChar);
            indiceSuccessiva = cercaParola(arrayParole, parolaSuccessiva, numeroDistinctParoleTesto);
            //  printf("\nHo trovato la parola %s all'indice %d", parolaSuccessiva, indiceSuccessiva);
            indicePrecedente = cercaParola(arrayParole, parolaPrecedente, numeroDistinctParoleTesto);

            Record occorrenzeParolaPrecedente = arrayRecordParole[indicePrecedente];
            int trovato = 0;
            for (int i = 0; (i < occorrenzeParolaPrecedente.numeroParoleSuccessive && !trovato); i++)
            {
                if (occorrenzeParolaPrecedente.occorrenze[i].parolaSuccessiva == indiceSuccessiva)
                {
                    occorrenzeParolaPrecedente.occorrenze[i].numeroOccorrenze = occorrenzeParolaPrecedente.occorrenze[i].numeroOccorrenze + 1;
                    occorrenzeParolaPrecedente.numeroParoleSuccessive = occorrenzeParolaPrecedente.numeroParoleSuccessive + 1;
                    trovato = 1;
                }
            }
            // non ho trovato occorrenza successiva:
            if (!trovato)
            {
                // 1 creo array più grande
                int newCountOccorrenze = occorrenzeParolaPrecedente.numeroParoleSuccessive + 1;
                Occorrenza *newOcc = malloc(sizeof(Occorrenza) * newCountOccorrenze);

                for (int i = 0; i < newCountOccorrenze - 1; i++)

                {
                    newOcc[i] = occorrenzeParolaPrecedente.occorrenze[i];
                }

                newOcc[newCountOccorrenze - 1].parolaSuccessiva = indiceSuccessiva;
                newOcc[newCountOccorrenze - 1].numeroOccorrenze = 1;

                free(occorrenzeParolaPrecedente.occorrenze);
                occorrenzeParolaPrecedente.occorrenze = newOcc;
                occorrenzeParolaPrecedente.numeroParoleSuccessive = 1;
                arrayRecordParole[indiceParolaInArrayStructParole - 1] = occorrenzeParolaPrecedente;
                printf("\nPrecedente:'%s'\tSuccessiva:'%s':\t num parole successive:%d", parolaPrecedente, parolaSuccessiva, arrayRecordParole->numeroParoleSuccessive);

                indiceParolaInArrayStructParole++;
            }

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

int getWord(char *word, int maxLen, FILE *f)
{
    int c, getch(void);
    char *w = word;
    while (isSeparator(c = fgetc(f)))
    {
        if (c != EOF)
            *w++ = c;
    }
    *w = '\0';
    return word[0];
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
    type_parola parola;

    while ((c = (fin[indexFin])) != '~')
    {
        if (isSeparator(c))
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
        indexFin++;
    }
    // inserisco fine Array
    strcpy(parola, "~");
    inserisciParola(arrayParole, parola, numeroDistinctParoleTesto);
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
