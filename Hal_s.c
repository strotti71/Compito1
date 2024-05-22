
#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>

#include "Hal_s.h"
#include "parameterHandler.h"
#include "string.h"
#include "fileHandler.h"
#include "stringHandler.h"
#include "occorrenza.h"
#include "printer.h"
/*
#include "fileHandler.c"
#include "stringHandler.c"
#include "occorrenza.c"
#include "printer.c"
#include "parameterHandler.c"


1) Apro il file di input
2) copio il contenuto del file in  array(char) fileNormalizzato che ha le seguenti proprietà
    - sono stati rimossi tutti i doppi spazi
    - la punteggiatura viene separata dalle parole precedenti e seguenti tramite uno spazio
3) chiudo il file di input
4) creo arrayParole che contiene le singole istanze di tutte le parole

*/

//***********************************************************************
//                                                                      *
//                    NOMENCLATURA PARTICOLARE PER VARIABILI            *
//                                                                      *
//   n_nomeVariabile:     intero che definisce un numero di occorrenze  *
//   i_nomeVariabile:     intero che definisce un indice                *
//                                                                      *
//***********************************************************************

/// @brief Funzione di ingresso al programma.
/// @param args il numero di argomenti passati in input
/// @param argv l'array di argomenti passati in input
/// @return
int main(int args, char *argv[])
{
    // setlocale(LC_ALL, "POSIX");

    init(args, argv, &parametri);
    produciTabellaOccorrenze();
    creaArrayProb();
    generaTesto();
}
int produciTabellaOccorrenze()
{
    // inizio la lettura del file:
    // la funzione crea un *char filenormalizzato in cui
    // tutte le parole e i segni di interpunzione sono separati da uno spazio
    fin = fopen(parametri.inputFileName, "r");
    if (fin == NULL)
    {
        printf("\n\nImpossibile aprire il file %s.\nEsco\n", parametri.inputFileName);
        return 1;
    }

    preparaStream(fin, fileNormalizzato);
    if (fin != NULL)
        fclose(fin);
    stampaArrayCaratteri(fileNormalizzato, lenFileNormalizzato, "File Normalizzato");
    popolaArrayParoleDistinte(fileNormalizzato);
    stampaArrayParole(arrayParoleDistinte, n_DistinctParoleTesto, "DIST");
    // costruisco l'array di record che contengono i conteggi delle parole successive
    popolaArrayRecordOccorrenze(fileNormalizzato);

    esportaCsv(arrayParoleDistinte, arrayRecordParole, n_DistinctParoleTesto, "export.csv");

    // liberare memoria
    free(arrayParoleDistinte);
    free(fileNormalizzato);

    return 0;
}
unsigned long long getMilliseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
}

void generaTesto()
{
    setlocale(LC_ALL, "it_IT.UTF8");
    type_parola_w parolaEstratta;
    type_parola_w parolaPrecedente;
    type_parola_w wStartingWord;
    type_parola_w upperParolaEstratta;
    mbstowcs(wStartingWord, parametri.startingWord, _MAX_LENGTH_WORD_);

    wcscpy(parolaPrecedente, wStartingWord);
    wcscpy(parolaEstratta, wStartingWord);

    printf("\nEcco il testo:\n");

    for (int x = 0; x < parametri.nParoleDaGenerare; x++)
    {
        int i = 0;
        int trovata = 0;

        // cerco l'indice della parola inizale nel recordProbability
        int indexParolaIniziale = cercaParolaArrayProbability(probabilityRecord, parolaEstratta, numeroParole);

        // genero un numero random tra 0 e 1
        srand(getMilliseconds());
        usleep(25000);
        double randNum = (double)rand() / RAND_MAX;
        while (i <= probabilityRecord[indexParolaIniziale].numeroParoleSuccessive && trovata == 0)
        {

            wcscpy(parolaEstratta, probabilityRecord[indexParolaIniziale].probabilityOccorrenze[i].parola);
            if (randNum <= probabilityRecord[indexParolaIniziale].probabilityOccorrenze[i].probability)
                trovata = 1;
            i++;
        }

        // se la parola precedente è punteggiatura,  inserisco spazio e primo carattere in maiuscolo

        switch (is_special_char(parolaPrecedente))
        {
        case 1:
            wcscpy(upperParolaEstratta, parolaEstratta);
            upperParolaEstratta[0] = towupper(upperParolaEstratta[0]);
            printf(" %ls", (wchar_t *)upperParolaEstratta);
            /* code */
            break;

        default:
            if (is_special_char(parolaEstratta) == 1)
                printf("%ls", (wchar_t *)parolaEstratta);
            else
                printf(" %ls", (wchar_t *)parolaEstratta);
            break;
        }
        fflush(stdout);
        wcscpy(parolaPrecedente, parolaEstratta);
    }
}

/*void creaArrayProb_old()
{

    // da,un,0.250.,qui,0.250.,dove,0.250.,ogni,0.250.
    // per il calcolo delle probabilità nell'array diventa
    // da,un,0.250.,qui,0.500.,dove,0.750.,ogni,1.000

    // 1. leggo i caratteri finchè incontro la prima virgola
    // 2. se virgola==1, ho trovato la parola guida
    // 3. se virgola >1
    //       - realloc di probabilityOccorrenza * virgolaù
    //       - probabilityOccorrenza[virgola].parola = word
    //       - probabilityOccorrenza.probability= probability+probability

    // Come individuare le parole ',':
    // se virgola è in posizione indexchar=0 allora virgola = word

    numeroParole = 1;               // il numero di parole guida nell'arrayProbability
    int virgola = 0;                // contatore delle virgole trovate nel testo
    int numeroParoleSuccessive = 0; // il numero di parole successive;
    type_parola word;               // la parola in costruzione durante la lettura
    char c;                         // carattere letto
    double prob = 0;                // probabilità cumulativa
    int indiceCarattereRiga = 0;    // cursore che indica la posizione sulla riga
    int isNextWord = 1;             // bool che indica la prossima sequenza di caratteri
                                    //       è una parola: 1
                                    //       è una probabilità : 0
    int indiceParola = 0;           // indice per la costruzione della parola

    // type_parola wordMemorizzata; // tengo in memoria l'ultima parola letta

    // ProbabilityOccorrenza *probabilityOccorrenza;

    printf("\n\ncarico conteggi:\n");
    // word = (char *)malloc(sizeof(char) * 100000);
    // wordMemorizzata = (char *)malloc(sizeof(char) * 100000);
    int numRighe;
    char **righe = leggiFile("export.csv", &numRighe);

    for (int i = 0; i < numRighe; i++)
    {
        printf("Riga %d: %s", i + 1, righe[i]);
    }
}
*/
/*char **leggiFile(const char *nomeFile, int *numRighe)
{
    FILE *file = fopen(nomeFile, "r");
    if (file == NULL)
    {
        perror("creaArrayProb");
        exit(EXIT_FAILURE);
    }

    char *righe[MAX_RIGHE];
    char *riga = malloc(MAX_LEN_RIGA * sizeof(char));
    if (riga == NULL)
    {
        perror("creaArrayProb");
        exit(EXIT_FAILURE);
    }

    int numrigheLette = 0;
    while (fgets(riga, MAX_LEN_RIGA, file) != NULL && numrigheLette < MAX_RIGHE)
    {
        righe[numrigheLette] = strdup(riga);
        printf(" %s\n", riga);
        numrigheLette++;
    }
    fclose(file);
    free(riga);
    *numRighe = numrigheLette;

    return righe;
}
*/

void creaArrayProb()
{

    // da,un,0.250.,qui,0.250.,dove,0.250.,ogni,0.250.
    // per il calcolo delle probabilità nell'array diventa
    // da,un,0.250.,qui,0.500.,dove,0.750.,ogni,1.000

    // 1. leggo i caratteri finchè incontro la prima virgola
    // 2. se virgola==1, ho trovato la parola guida
    // 3. se virgola >1
    //       - realloc di probabilityOccorrenza * virgolaù
    //       - probabilityOccorrenza[virgola].parola = word
    //       - probabilityOccorrenza.probability= probability+probability

    // Come individuare le parole ',':
    // se virgola è in posizione indexchar=0 allora virgola = word

    numeroParole = 1;               // il numero di parole guida nell'arrayProbability
    int virgola = 0;                // contatore delle virgole trovate nel testo
    int numeroParoleSuccessive = 0; // il numero di parole successive;
    type_parola_w word;             // la parola in costruzione durante la lettura
    wchar_t c;                      // carattere letto
    double prob = 0;                // probabilità cumulativa
    int indiceCarattereRiga = 0;    // cursore che indica la posizione sulla riga
    int isNextWord = 1;             // bool che indica la prossima sequenza di caratteri
                                    //       è una parola: 1
                                    //       è una probabilità : 0
    int indiceParola = 0;           // indice per la costruzione della parola
    int letturaVirgola = 0;         // bool per il controllo se la parola è una virgola e non un separatore nel file
    type_parola_w wordMemorizzata;  // tengo in memoria l'ultima parola letta

    FILE *fileCSV = fopen("export.csv", "r");
    if (fileCSV == NULL)
    {
        perror("creaArrayProb");
        EXIT_FAILURE;
    }
    pulisciStringa(word, _MAX_LENGTH_WORD_);
    pulisciStringa(wordMemorizzata, _MAX_LENGTH_WORD_);
    // preparazione: conto il numero di righe per allocare la lunghezza dell'array delle parole
    int numeroRighe = contarighe(fileCSV);
    probabilityRecord = (ProbabilityRecord *)calloc(numeroRighe, sizeof(ProbabilityRecord));
    printf("\n\ncarico conteggi:\n");
    setlocale(LC_ALL, "C");
    if (probabilityRecord == NULL)
    {
        perror("ProbabilityRecord?");
        exit(1);
    }
    while ((c = fgetc(fileCSV)) != EOF)

    {
        switch (c)
        {
        case '\n':
            if (!isNextWord) // ho terminato di leggere le occorrenze; devo memorizzare l'ultima occorrenza
            {

                double number = wCharToDouble(word);
                prob = (double)(prob + number);
                if (prob > 0.99)
                    prob = 1;
                ProbabilityOccorrenza *probabilityOccorrenzetmp = realloc(probabilityRecord[numeroParole - 1].probabilityOccorrenze, sizeof(ProbabilityOccorrenza) * numeroParoleSuccessive);

                // ho trovato la prima parola: la inserisco nel prob array
                wcscpy(probabilityOccorrenzetmp[numeroParoleSuccessive - 1].parola, wordMemorizzata);
                probabilityOccorrenzetmp[numeroParoleSuccessive - 1].probability = prob;
                probabilityRecord[numeroParole - 1].numeroParoleSuccessive = numeroParoleSuccessive;
                probabilityRecord[numeroParole - 1].probabilityOccorrenze = probabilityOccorrenzetmp;

                virgola++;
                printf(" %ls (%f)", probabilityRecord[numeroParole - 1].probabilityOccorrenze[numeroParoleSuccessive - 1].parola,
                       probabilityRecord[numeroParole - 1].probabilityOccorrenze[numeroParoleSuccessive - 1].probability);

                fflush(stdout);
                pulisciStringa(word, _MAX_LENGTH_WORD_);
                pulisciStringa(wordMemorizzata, _MAX_LENGTH_WORD_);
                // free(probabilityOccorrenzetmp);
                isNextWord = 1;
                indiceCarattereRiga++;
            }
            // terminata di leggere la riga ripartono i contatori di riga e di virgola
            indiceCarattereRiga = 0;
            numeroParoleSuccessive = 0;
            numeroParole++;
            indiceParola = 0;
            virgola = 0;
            isNextWord = 1;
            prob = 0;
            pulisciStringa(word, _MAX_LENGTH_WORD_);
            break;
        case ',':
            if (virgola == 0 && indiceCarattereRiga == 0)
            {
                //  CASO SPECIALE: LA VIRGOLA LETTA È UNA PAROLA E NON
                // UN SEPARATORE
                indiceCarattereRiga++;
                indiceParola++;
                // word = (char *)realloc(word, indiceCarattereRiga * sizeof(char));
                appendCharToString(word, c, indiceParola - 1);
                break;
            }
            if (virgola == 0 && indiceCarattereRiga > 0)
            {
                // PRIMA VIRGOLA INCONTRATA
                //  INDICA LA FINE DELLA PAROLA GUIDA E L'INIZIO DELLE OCCORRENZE
                //  L'ARRAY DELLE OCCORRENZE VIENE INZIALIZZATO A 1 (MALLOC(1))
                numeroParoleSuccessive++;

                appendCharToString(word, L'\0', indiceCarattereRiga);
                indiceParola = 0;
                ProbabilityRecord pRecordTemp = probabilityRecord[numeroParole - 1];
                // ProbabilityOccorrenza *probabilityOccorrenzetmp = realloc(probabilityRecord[numeroParole - 1].probabilityOccorrenze, sizeof(ProbabilityOccorrenza) * numeroParoleSuccessive);
                //  ho trovato la prima parola: la inserisco nel prob array
                wcscpy(pRecordTemp.parola, word);
                pRecordTemp.numeroParoleSuccessive = numeroParoleSuccessive;
                probabilityRecord[numeroParole - 1] = pRecordTemp;
                //      free(pRecordTemp.probabilityOccorrenze);
                virgola++;

                pulisciStringa(word, _MAX_LENGTH_WORD_);
                // probabilityRecord = probTemp;
                printf("\n[%ls] ", probabilityRecord[(numeroParole - 1)].parola);
                fflush(stdout);

                indiceCarattereRiga++;
                break;
            }

            if (virgola > 0)
            {
                if (wcscmp(wordMemorizzata, word) == 0) // se la parola precedente è una virgola allora la virgola è la parola successiva
                {
                    letturaVirgola = 1;
                    indiceParola++;
                    appendCharToString(word, c, indiceParola - 1);
                    printf("%ls", word);
                    fflush(stdout);
                    wcscpy(wordMemorizzata, word);

                    isNextWord = 0;
                    indiceCarattereRiga++;
                    indiceParola = 0;
                    pulisciStringa(word, _MAX_LENGTH_WORD_);
                }

                else if (isNextWord) // devo leggere una parola
                {
                    // ho incontrato la parola di un'occorrenza: registro la parola in wordMemory per
                    // registrarla successivamente  nell'arrayProbOccorrenza con la prio
                    wcscpy(wordMemorizzata, word);
                    virgola++;
                    isNextWord = 0;
                    indiceCarattereRiga++;
                    indiceParola = 0;
                    pulisciStringa(word, _MAX_LENGTH_WORD_);
                }
                else if (letturaVirgola == 0) // leggo l'occorrenza
                {

                    double number = wCharToDouble(word);
                    prob = (double)(prob + number);

                    if (prob > 0.99)
                        prob = 1;
                    ProbabilityOccorrenza *probabilityOccorrenzetmp = realloc(probabilityRecord[numeroParole - 1].probabilityOccorrenze, sizeof(ProbabilityOccorrenza) * numeroParoleSuccessive);

                    // ho trovato la prima parola: la inserisco nel prob array
                    wcscpy(probabilityOccorrenzetmp[numeroParoleSuccessive - 1].parola, wordMemorizzata);
                    probabilityOccorrenzetmp[numeroParoleSuccessive - 1].probability = prob;
                    probabilityRecord[numeroParole - 1].numeroParoleSuccessive = numeroParoleSuccessive;
                    probabilityRecord[numeroParole - 1].probabilityOccorrenze = probabilityOccorrenzetmp;

                    virgola++;
                    printf(" %ls (%f)", probabilityRecord[numeroParole - 1].probabilityOccorrenze[numeroParoleSuccessive - 1].parola,
                           probabilityRecord[numeroParole - 1].probabilityOccorrenze[numeroParoleSuccessive - 1].probability);

                    fflush(stdout);
                    pulisciStringa(word, _MAX_LENGTH_WORD_);
                    pulisciStringa(wordMemorizzata, _MAX_LENGTH_WORD_);
                    isNextWord = 1;
                    indiceCarattereRiga++;
                    numeroParoleSuccessive++;
                    indiceParola = 0;
                }
            }

            break;
        default:
            letturaVirgola = 0;
            indiceCarattereRiga++;
            indiceParola++;
            // word = (char *)realloc(word, indiceCarattereRiga * sizeof(char));
            appendCharToString(word, c, indiceParola - 1);
            break;
        }
    }

    numeroParole--;
    printf("\n\nStampa TUTTO\n");
    // stampo tutto
    for (int i = 0; i < numeroParole - 1; i++)
    {
        wprintf(L"\n%ls (%d) : ", probabilityRecord[i].parola, probabilityRecord[i].numeroParoleSuccessive);
        for (int j = 0; j < probabilityRecord[i].numeroParoleSuccessive; j++)
        {
            wprintf(L"  (%ls) %f", probabilityRecord[i].probabilityOccorrenze[j].parola,
                    probabilityRecord[i].probabilityOccorrenze[j].probability);
            fflush(stdout);
        }
    }
}

/// @brief funzione per l'inizializzazione del programma.
/// la funzione chiama leggiParametri per l'identificazione dei parametri in input e li stampa a video trmite la funct stampaParametri
/// @param args il numero di argomenti inseriti dall'utente
/// @param argv l'array di argomenti inseriti dall'utente
/// @param parametri la struttura per memorizzare gli argomenti del programma
void init(int args, char *argv[], ParametriInput *parametri)
{
    //***********************************************
    //                   INIT                       *
    // funct che richiama le seguenti funzioni:     *
    //  leggiParametri: analizza e assegna i valori *
    //  letti dai parametri in input e li stampa    *
    //***********************************************
    leggiParametri(args, argv, parametri);
    stampaParametri(parametri);
}

/// @brief   il metodo legge il file e copia tutte le parole nell'array fileNormalizzato eseguendo le seguenti correzioni:
/// 1- conto i caratteri del file in input (serviranno per dimensionare fileNormalizzato
/// 1- inserisce spazi prima e dopo la punteggiatura
/// 2- elimina doppi spazi
/// al termine dell'elaborazione ciascuna parola o segno di punteggiatura sono separati da uno spazio
/// @param fin il file in input
/// @param out il file normalizzato
void preparaStream(FILE *fin, wchar_t *out)
{
    setlocale(LC_ALL, "");
    wchar_t c;
    wchar_t cPrec = ' ';

    n_CharFileNormalizzato = 0;
    lenFileNormalizzato = contaCaratteri(fin);
    rewind(fin);

    out = malloc(sizeof(wchar_t) * lenFileNormalizzato);

    inserisciCarattere(&out, '.', &n_CharFileNormalizzato);
    inserisciCarattere(&out, ' ', &n_CharFileNormalizzato);

    while ((c = fgetwc(fin)) != EOF)
    {
        if (isPunteggiatura(c))
        {
            if (cPrec != ' ')
            {
                inserisciCarattere(&out, 32, &n_CharFileNormalizzato);
                cPrec = ' ';
            }

            inserisciCarattere(&out, c, &n_CharFileNormalizzato);
            inserisciCarattere(&out, 32, &n_CharFileNormalizzato);
            cPrec = ' ';
        }
        else if (c < 32)
        {
            if (cPrec != ' ')
            {
                inserisciCarattere(&out, 32, &n_CharFileNormalizzato);
                cPrec = ' ';
            }
        }
        else
        {
            // se il carattere precedente e il carattere letto sono entrambi spazi,
            // non inserisco il carattere per evetiare doppi spazi inutili
            c = towlower(c);
            if (!((cPrec == c) && (c == 32)))
            {
                inserisciCarattere(&out, c, &n_CharFileNormalizzato);
            }
            cPrec = c;
        }
    }
    fileNormalizzato = out;
}

// Funzione di esempio per aggiungere un carattere alla fine della stringa
void appendCharToStringNew(wchar_t *arr, wchar_t c, int n_CharFileNormalizzato)
{
    arr[n_CharFileNormalizzato] = c;
    // arr[n_CharFileNormalizzato + 1] = L'\0'; // Aggiungi il terminatore null
}

// Funzione che inserisce un carattere in un array ridimensionandolo se necessario
void inserisciCarattere(wchar_t **arr, wchar_t c, int *n_CharFileNormalizzato)
{
    // Se la lunghezza di fileNormalizzato è minore dell'indice di lettura
    // espandi l'array di un carattere
    if (lenFileNormalizzato == *n_CharFileNormalizzato)
    {
        lenFileNormalizzato += 1;
        wchar_t *arrTmp = (wchar_t *)malloc(sizeof(wchar_t) * (lenFileNormalizzato + 1)); // +1 per il terminatore null
        if (arrTmp == NULL)
        {
            perror("Impossibile allocare la memoria");
            exit(EXIT_FAILURE);
        }
        wmemcpy(arrTmp, *arr, *n_CharFileNormalizzato);
        free(*arr);    // Libera la memoria precedente
        *arr = arrTmp; // Assegna il nuovo puntatore all'originale
    }

    appendCharToStringNew(*arr, c, *n_CharFileNormalizzato);
    *n_CharFileNormalizzato += 1;
}
/*
void inserisciCarattere(wchar_t *arr, wchar_t c, int *n_CharFileNormalizzato)
{
    // se la lunghezza di fileNormalizzato è minore dell'indice di lettura
    // espando l'array di un carattere
    if (lenFileNormalizzato == *n_CharFileNormalizzato)
    {
        lenFileNormalizzato += 1;
        wchar_t *arrTmp = (wchar_t *)malloc(sizeof(wchar_t) * (lenFileNormalizzato));
        if (arrTmp == NULL)
        {
            perror("Impossibile allocare la memoria");
        }
        wmemcpy(arrTmp, arr, lenFileNormalizzato);
        appendCharToString(arrTmp, c, *n_CharFileNormalizzato);
        arr = arrTmp;
        free(arrTmp);
    }
    else
        appendCharToString(arr, c, *n_CharFileNormalizzato);
    *n_CharFileNormalizzato += 1;
}
*/
void popolaArrayRecordOccorrenze(wchar_t *testo)
{
    ///   /* seconda lettura del file: popolo l'array di record
    ///   per ciascuna parola che leggo nel file:
    ///   - cerco l'indice sull'arrayParole
    ///      (es: quel = 1)
    ///      cerco l'indice in record[].parola
    ///          - se non è presente inserisco l'indice in coda all'array
    ///   da terminare per le parole successive e le occorrenze

    wchar_t c;
    int i_Char = 0;
    int i_Testo = 0;

    int i_Precedente = -1;
    int i_Successiva;

    int indiceParolaInArrayStructParole = 0;
    type_parola_w parolaPrecedente = L"."; // l'ultima parola letta
    type_parola_w parolaSuccessiva;        // la parola che sto leggendo

    pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

    while (i_Testo < n_CharFileNormalizzato)
    {
        c = testo[i_Testo];
        if (isSeparator(c))
        {
            if (i_Testo > 1) // se sono nel primo carattere '.' non ho parole precedenti
            {
                // chiudo la parola che sto costruendo
                appendCharToString(parolaSuccessiva, L'\0', i_Char);

                // cerco l'indice della parola appena letta nella'rray parole
                i_Successiva = cercaParola(arrayParoleDistinte, parolaSuccessiva, n_DistinctParoleTesto);

                // cerco l'indice della parola precedente
                i_Precedente = cercaParola(arrayParoleDistinte, parolaPrecedente, n_DistinctParoleTesto);
                // creo un nuovo record copia del record corrispondente alla parola precedente

                Record RecordParolaTemp = arrayRecordParole[i_Precedente];

                int trovato = 0;
                for (int i = 0; (i < RecordParolaTemp.n_ParoleSuccessive && !trovato); i++)
                {
                    if (RecordParolaTemp.occorrenze[i].parolaSuccessiva == i_Successiva)
                    {
                        RecordParolaTemp.occorrenze[i].n_Occorrenze = RecordParolaTemp.occorrenze[i].n_Occorrenze + 1; //= arrayRecordParole->occorrenze[i].numeroOccorrenze + 1;
                        RecordParolaTemp.totaleParoleSuccessive++;                                                     //= arrayRecordParole->n_ParoleSuccessive + 1;
                        trovato = 1;
                        arrayRecordParole[i_Precedente] = RecordParolaTemp;
                    }
                }
                // non ho trovato occorrenza successiva:
                if (!trovato)
                {
                    // 1 creo array più grande
                    int newCountOccorrenze = RecordParolaTemp.n_ParoleSuccessive + 1;
                    if (newCountOccorrenze == 1)
                    {
                        RecordParolaTemp.occorrenze = malloc(sizeof(Occorrenza) * 1);
                        if (!RecordParolaTemp.occorrenze)
                        {
                            perror("Malloc fallita...");
                            exit(EXIT_FAILURE);
                        }
                    }

                    else
                        // rialloco la memoria per arrayTemp.occorrenze[];
                        RecordParolaTemp.occorrenze = (Occorrenza *)realloc(RecordParolaTemp.occorrenze, newCountOccorrenze * sizeof(Occorrenza));

                    if (!RecordParolaTemp.occorrenze)
                    {
                        perror("realloc fallita...");
                        exit(EXIT_FAILURE);
                    }

                    // printf("\n%s %s %d\n", parolaPrecedente, parolaSuccessiva, RecordParolaTemp.occorrenze[newCountOccorrenze].numeroOccorrenze);
                    // fflush(stdout);

                    RecordParolaTemp.occorrenze[newCountOccorrenze - 1].parolaSuccessiva = i_Successiva;
                    RecordParolaTemp.occorrenze[newCountOccorrenze - 1].n_Occorrenze = 1;
                    RecordParolaTemp.n_ParoleSuccessive++;
                    RecordParolaTemp.totaleParoleSuccessive++;
                    indiceParolaInArrayStructParole++;

                    arrayRecordParole[i_Precedente] = RecordParolaTemp;
                }
                // rialloco lo spazio per il record indice precedente

                // printf("\n%d\n", arrayRecordParole[indicePrecedente].occorrenze[1].numeroOccorrenze);
                //  fflush(stdout);

                pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
                wcscpy(parolaPrecedente, parolaSuccessiva);
                pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

                i_Testo++;
                i_Char = 0;
            }
            else
            {
                i_Testo++;
                i_Char = 0;
            }
        }
        else
        {
            if (i_Char < _MAX_LENGTH_WORD_)
            {
                parolaSuccessiva[i_Char] = c;
                i_Char++;
                i_Testo++;
            }
            else
                printf("BREAK");
        }
    }
}

/// @brief il metodo legge il file fino alla fine e scrive tutte le parole trovate in un distinctArray.
/// L'arrayParoleDistinte contiene ciascuna parola del testo letto una sola volta;
/// @param fin
void popolaArrayParoleDistinte(wchar_t *fin)
{
    wchar_t c;
    int i_Char = 0;
    int i_Fin = 0;
    type_parola_w parola;
    pulisciStringa(parola, _MAX_LENGTH_WORD_);

    arrayParoleDistinte = (type_parola_w *)calloc(1, sizeof(type_parola_w));
    if (arrayParoleDistinte == NULL)
    {
        perror("popolaArrayParoleDistinte: impossibile allocare spazio per il distinct array");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < lenFileNormalizzato; i++)
    {
        c = (fin[i_Fin]);
        if (!((c < 32)))
        {
            if (isSeparator(c))
            {
                // se ho trovato un separatore ho una nuova parola e la inserisco nell'array

                if (i_Char > 0)
                {
                    appendCharToString(parola, L'\0', i_Char);
                    n_ParoleTotali++;

                    if (cercaParola(arrayParoleDistinte, parola, n_DistinctParoleTesto) < 0)
                    {
                        n_DistinctParoleTesto++;

                        void *tmpParoleDistinte = (type_parola_w *)realloc(arrayParoleDistinte, sizeof(type_parola_w) * n_DistinctParoleTesto);
                        //  arrayParoleDistinte = (type_parola *)realloc(arrayParoleDistinte, sizeof(type_parola) * n_DistinctParoleTesto);
                        inserisciParola(tmpParoleDistinte, parola, n_DistinctParoleTesto - 1);
                        arrayParoleDistinte = tmpParoleDistinte;
                    }
                    pulisciStringa(parola, _MAX_LENGTH_WORD_);
                    i_Char = 0;
                }
            }
            else if (isPunteggiatura(c))
            {
                appendCharToString(parola, c, i_Char);
                //  parola[i_Char] = c;
                i_Char++;
                appendCharToString(parola, L'\0', i_Char);
                n_ParoleTotali++;

                if (cercaParola(arrayParoleDistinte, parola, n_DistinctParoleTesto) < 0)
                {
                    n_DistinctParoleTesto++;
                    arrayParoleDistinte = (type_parola_w *)realloc(arrayParoleDistinte, sizeof(type_parola_w) * n_DistinctParoleTesto);
                    inserisciParola(arrayParoleDistinte, parola, n_DistinctParoleTesto - 1);
                }
                pulisciStringa(parola, _MAX_LENGTH_WORD_);
                i_Char = 0;
            }
            else
            {
                if (i_Char < _MAX_LENGTH_WORD_)
                {
                    parola[i_Char] = c;
                    i_Char++;
                }
            }
        }
        i_Fin++;
    }
}

/*void cestino(){



    // creo arrayRecordParole dimensionato con il numero di parole distinte presenti nel testo
    // arrayRecordParole = calloc(n_DistinctParoleTesto, sizeof(Record));
    // arrayRecordParole->n_ParoleSuccessive = 0;



        stampaArrayCaratteri(fileNormalizzato, lenFileNormalizzato, "file normalizzato: ");
    printf("\n\n");

    popolaArrayParoleDistinte(fileNormalizzato);

    stampaArrayParole(arrayParoleDistinte, n_DistinctParoleTesto, "\nParole distinte del testo: ");

    printf("\nNumero Parole distinte: %d\nNumero Parole Totali: %d\n", n_DistinctParoleTesto, n_ParoleTotali);

//stampa statistiche delle parole
        int index = 0;
    while (index < n_DistinctParoleTesto)
    {
        printf("\n%s", arrayParoleDistinte[index]);
        for (int j = 0; j < arrayRecordParole[index].n_ParoleSuccessive; j++)
        {
            double d = calcolaOccorrenze(arrayRecordParole[index].occorrenze[j].n_Occorrenze, arrayRecordParole[index].totaleParoleSuccessive);
            printf(",%s,%5.3f.", getParola(arrayParoleDistinte, arrayRecordParole[index].occorrenze[j].parolaSuccessiva), d);
            fflush(stdout);
        }
        index++;
    }
}*/