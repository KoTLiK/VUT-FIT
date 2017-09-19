/**
 * @file       wordcount.c
 * @brief      Riesenie IJC-DU2-2, 22.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Program pocita cetnost slov ve vstupnim textu
 */

#include "htable.h"
#include "io.h"
#include <stdio.h>

/**
 * V literature som sa docital, ze vhodna velkost tabulky je co najvecsia, pretoze
 * sa znizuje sanca na koliziu. Kedze nemozme mat nekonecne velku tabulku, je vhodne
 * pouzit nejake prvocislo, ktore narozdiel od ostatnych cisel znizuje kolizie aj
 * pri mensich hodnotach. Inspiroval som sa tabulkou
 *     "http://planetmath.org/goodhashtableprimes", kde som si vybral 49157.
 */
#define HTAB_SIZE 49157
#define WORD_LIMIT 128

/**
 * @brief      Prints key and data from hash table
 *
 * @param      key   Word
 * @param[in]  data  Number of words (key)
 */
void htab_print(char *key, unsigned data)
{
	printf("%s\t%d\n", key, data);
}

/* ================================= START ================================= */
int main(void)
{
	htab_t *table;

	if ((table = htab_init(HTAB_SIZE)) == NULL)
	{
		fprintf(stderr, "CHYBA: Nepodarila sa inicializacia tabulky\n");
		return 1;
	}


	int flag = 1;
	int lenght = 0;
	char word[WORD_LIMIT+1];
	htab_listitem_t *item = NULL;

	while ((lenght = get_word(word, WORD_LIMIT, stdin)) != EOF)
	{
		if (lenght < EOF) break; // get_word Error

		if (lenght >= WORD_LIMIT)
		{
			if (flag)
			{
				flag = 0;
				fprintf(stderr, "CHYBA: Prekroceny limit %d znakov na slovo\n", WORD_LIMIT-1);
			}
			word[WORD_LIMIT-1] = '\0'; // Longer words are reduced
		}

		if ((item = htab_lookup_add(table, word)) == NULL)
		{
			fprintf(stderr, "CHYBA: Nepodarila sa inicializacia pre novy zaznam\n");
			htab_free(table);
			return 1;
		}

		item->data++;
	}

	// Printing result
	htab_foreach(table, htab_print);

	// Safe ending ;-)
	htab_free(table);
	return 0;
}
/* ================================== END ================================== */
