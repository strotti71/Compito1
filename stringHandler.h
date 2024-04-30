int isSeparator(char c);
int isPunteggiatura(char c);
char *appendCharToString(char *s, char c, int i);
void resettaParole(char *parolaPrecedente, char *parolaSuccessiva, int indexChar);
void pulisciStringa(char *s, int lunghezza);

#define _MAX_LENGTH_WORD_ 31 // lunghezza massima della parola (30 char + \0)
typedef char type_parola[_MAX_LENGTH_WORD_];