/**
 * @file       htab_clear.c
 * @brief      Riesenie IJC-DU2-2, 21.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Zruseni vsech polozek v tabulce
 */

#include "htable.h"
#include <stdlib.h>

void htab_clear(htab_t *table)
{
	if (table == NULL) return;

	htab_listitem_t *item;
	htab_listitem_t *item_p;

	for (unsigned i = 0; i < table->htab_size ; i++)
		if (table->list[i] != NULL)
		{
			item = table->list[i];
			while (item != NULL)
			{
				item_p = item->next;
				free(item->key);
				free(item);
				item = item_p;
			}
		}
}
