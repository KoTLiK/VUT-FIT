/**
 * @file       htab_remove.c
 * @brief      Riesenie IJC-DU2-2, 21.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Vyhledani a zruseni zadane polozky
 */

#include "htable.h"
#include <stdlib.h>
#include <string.h>

void htab_remove(htab_t *table, char *key)
{
	if (table == NULL) return;

	unsigned index = table->hash_fun_ptr(key, table->htab_size);
	htab_listitem_t *item = table->list[index];
	htab_listitem_t *item_p = NULL;

	while (item != NULL)
	{
		if (strcmp(item->key, key) == 0) // Match
		{
			if (item_p == NULL) // First item
				table->list[index] = item->next;
			else // Inside of list
				item_p->next = item->next;

			free(item->key);
			free(item);

			table->n--;
			return;
		}

		// Iteration update
		item_p = item;
		item = item->next;
	}
}
