/**
 * @file       io.c
 * @brief      Riesenie IJC-DU2-2, 19.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Cte jedno slovo ze souboru f do zadaneho pole znaku
 *     a vrati delku slova (z delsich slov nacte prvnich max-1 znaku,
 *     a zbytek preskoci). Funkce vraci EOF, pokud je konec souboru.
 */

#include "io.h"

int get_word(char *s, int max, FILE *f)
{
	// Invalid pointer; Error value -2
	if (f == NULL) return -2;
	// Invalid limit; Error value -3
	if (max <= 0) return -3;

	int c, i = 0;

	while ((c = fgetc(f)) != EOF && i < max)
		if (isspace(c))
		{
			if (i != 0) break; // Space behind at least 1 character
		}
		else s[i++] = c;

	if (i == max) // Ignoring remaining part of word
		while ((c = fgetc(f)) != EOF)
			if (isspace(c)) break; // End of word


	if (!i) return EOF;

	s[i] = '\0'; // End of string

	return i;
}
