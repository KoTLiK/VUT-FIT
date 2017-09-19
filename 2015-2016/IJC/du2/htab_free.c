/**
 * @file       htab_free.c
 * @brief      Riesenie IJC-DU2-2, 21.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Zruseni cele tabulky
 */

#include "htable.h"
#include <stdlib.h>

void htab_free(htab_t *table)
{
	if (table == NULL) return;

	htab_clear(table);
	free(table);

	table = NULL;
}
