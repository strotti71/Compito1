#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

char stringone[1000000]; // array in cui viene
Record arrayrecordParole[1000000];

void popolaArrayParole(char *fin);
void popolaArrayRecordOccorrenze(char *fin);
char *preparaStream(FILE *fin);
// int parolaRegistrata(char parola[], Record *record, int numeroParoleTrovate);
// void stampaStatistiche(Record *arrayRecord, int numeroParoleTrovate);

char *getNomeFile(int args, char *argv[]); // funzione che restituisce il nome file da argomento di avvio
void leggiFile(FILE *fin);

char nomeFile[] = "ciao.txt"; // file di testo di default

char *fileNormalizzato;

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

    leggiFile(fin);

    // chiudo il file
    if (fin != NULL)
    {
        printf("\nChiudo il file %s\n", nomeFile);
        fclose(fin);
    }

    popolaArrayParole(fileNormalizzato);

    printf("\n\n");

    stampaArray(arrayParole, numeroDistinctParoleTesto);

    printf("\n\nNumero Parole distinte: %d\nNumero Parole Totali: %d\n", numeroDistinctParoleTesto, numeroParoleTotali);
    fflush(stdout);

    // costruisco l'array di record che contengono i conteggi delle parole successive
    popolaArrayRecordOccorrenze(fileNormalizzato);

    for (int i = 0; i < 1000; i++)
        printf("-%d-", arrayrecordParole[i].parola);

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
    int index = 0;

    while ((c = fgetc(fin)) != EOF)
    {
        if (isPunteggiatura(c))
        {
            if (cPrec != ' ')
            {
                appendCharToString(stringone, ' ', index);
                index++;
                cPrec = ' ';
            }
            appendCharToString(stringone, c, index);
            index++;
            appendCharToString(stringone, ' ', index);
            index++;
            cPrec = ' ';
        }
        else
        {
            // se il carattere precedente e il carattere letto sono entrambi spazi,
            // non inserisco il carattere per evetiare doppi spazi inutili
            if ((cPrec == c) != ' ')
                appendCharToString(stringone, c, index);
            index++;
            cPrec = c;
        }
    }
    stringone[++index] = '~';
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

    int indiceParolaInArraydistinctParole;
    int indiceParolaInArrayStructParole = 0;
    type_parola parolaPrecedente = " ";
    type_parola parolaSuccessiva;

    pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
    pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

    while ((testo[indexTesto]) != '~')
    {
        c = testo[indexTesto];
        if (isSeparator(c))
        {
            // chiudo la parola che sto costruendo

            appendCharToString(parolaSuccessiva, '\0', indexChar);
            indiceParolaInArraydistinctParole = cercaParola(arrayParole, parolaSuccessiva, numeroDistinctParoleTesto);
            printf("\nHo trovato la parola %s all'indice %d", parolaSuccessiva, indiceParolaInArraydistinctParole);

            // cerco l'indice della parola nell arrayRecordParole:
            //   se non trovo inserisco l'indice
            int ind = cercaIntero(arrayrecordParole, indiceParolaInArraydistinctParole, indiceParolaInArrayStructParole);
            if (ind < 0)
                arrayrecordParole[indiceParolaInArrayStructParole].parola = indiceParolaInArraydistinctParole;
            fflush(stdout);
            pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
            strcpy(parolaPrecedente, parolaSuccessiva);
            pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
            indexTesto++;
            indexChar = 0;
        }
        /*  else if (isPunteggiatura(c))
          {
              indiceParolaInArraydistinctParole = cercaParola(arrayParole, parolaPrecedente, numeroDistinctParoleTesto);
              printf("\nHo trovato la parola %s all'indice %d", parolaPrecedente, indiceParolaInArraydistinctParole);
              pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
              appendCharToString(parolaPrecedente, c, 0);
              appendCharToString(parolaPrecedente, '\0', 1);
              indiceParolaInArraydistinctParole = cercaParola(arrayParole, parolaPrecedente, numeroDistinctParoleTesto);
              printf("\nHo trovato la parola %s all'indice %d", parolaPrecedente, indiceParolaInArraydistinctParole);

              pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
              indexChar = 0;
          }*/
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

    while ((c = (fin[indexFin])) != '\0')
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
