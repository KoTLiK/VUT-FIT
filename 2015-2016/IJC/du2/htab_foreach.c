/**
 * @file       htab_foreach.c
 * @brief      Riesenie IJC-DU2-2, 21.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Volani funkce fn pro kazdy prvek
 */

#include "htable.h"
#include <stdlib.h>

void htab_foreach(htab_t *table, void (*fn)(char *key, unsigned value))
{
	if (table == NULL) return;

	htab_listitem_t *item;

	for (unsigned i = 0; i < table->htab_size ; i++)
		if (table->list[i] != NULL)
			for (item = table->list[i]; item != NULL; item = item->next)
				fn(item->key, item->data);
}
