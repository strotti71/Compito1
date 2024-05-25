
#include "stringHandler.h"

//***********************************************************************
//                                                                      *
//                    LIBRERIA PER LA GESTIONE DELLE STRINGHE           *
//                                                                      *
//***********************************************************************

/// @brief funzione per pulire un array di caratteri wchar_t. la funzione imposta tutti gli elementi dell'array a zero
/// @param s            l'array da pulire
/// @param lunghezza    lunghezza dell'array
void pulisciStringa(wchar_t *s, int lunghezza)
{
    for (int i = 0; i < lunghezza; i++)
    {
        s[i] = 0;
    }
}

/// @brief funzione per pulire un array di caratteri char. la funzione imposta tutti gli elementi dell'array a zero
/// @param s            l'array da pulire
/// @param lunghezza    lunghezza dell'array
void pulisciStringaChar(char *s, int lunghezza)
{
    for (int i = 0; i < lunghezza; i++)
    {
        s[i] = 0;
    }
}

/// @brief funzione per identificare se un carattere letto è un separatore (spazio o ritorno a capo)
/// @param c    il carattere da investigare
/// @return     restituisce 1 se il carattere è un separatore, 0 altrimenti
int isSeparator(wchar_t c)
{
    return c == ' ' || c == '\n';
}

/// @brief funzione per identificare se un carattere letto è un segno di interpunzione ( . , ; ? ! ;)
/// @param c    il carattere da investigare
/// @return     restituisce 1 se il carattere è un segno di interpunzione, 0 altrimenti
int isPunteggiatura(wchar_t c)
{
    return c == ',' || c == ';' || c == '.' || c == '!' || c == ':' || c == '?';
}

/// @brief metodo per aggiungere un carattere a una stringa,  1.creo la stringa temporanea    2.alloco le dimensioni della stringa originale+ le dim di un carattere   3. copio nella stringa temporanea la concatenazione    restituisco la stringa temporanea
/// @param s la stringa a cui va aggiunto il carattere
/// @param c il carattere da aggiungere
/// @param i la osizione in cui aggiungere il carattere
void appendCharToString(wchar_t *s, wchar_t c, int i)
{
    s[i] = c;
}

int is_special_char(const type_parola_w parola)
{
    // Controlla se parola è uno dei caratteri speciali
    if (wcscmp(parola, L".") == 0 ||
        wcscmp(parola, L":") == 0 ||
        wcscmp(parola, L"?") == 0 ||
        wcscmp(parola, L"!") == 0)
    {
        return 1;
    }
    return 0;
}
// Funzione di esempio per aggiungere un carattere alla fine della stringa
void appendCharToStringNew(wchar_t *arr, wchar_t c, int n_CharFileNormalizzato)
{
    arr[n_CharFileNormalizzato] = c;
}

/// @brief funzione per pulire due parole
/// @param parolaPrecedente prima parola da pulire
/// @param parolaSuccessiva seconda parola da pulire
/// @param indexChar
/*void resettaParole(wchar_t *parolaPrecedente, wchar_t *parolaSuccessiva, int indexChar)
{
    pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
    wcscpy(parolaPrecedente, parolaSuccessiva);
    pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);
}*/