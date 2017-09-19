/**
 * @file       htab_lookup_add.c
 * @brief      Riesenie IJC-DU2-2, 22.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Vyhleda zaznam odpovidajici retezci "key" a
 *         - pokud jej nalezne, vrati ukazatel na zaznam
 *         - pokud nenalezne, automaticky prida zaznam a vrati ukazatel
 */

#include "htable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

htab_listitem_t * htab_lookup_add(htab_t *table, char *key)
{
	if (table == NULL) return NULL;

	unsigned index = table->hash_fun_ptr(key, table->htab_size);
	htab_listitem_t *item = table->list[index];

	for ( ; item != NULL; item = item->next)
		if (strcmp(item->key, key) == 0)
			return item; // Match


	// Memmory for item structure
	if ((item = calloc(1, sizeof(struct htab_listitem) )) == NULL)
		return NULL;

	// Memmory for key "string"
	if ((item->key = calloc(strlen(key)+1, sizeof(char) )) == NULL)
	{
		item = (free(item), NULL);
		return NULL;
	}


	// Copyrights  ;)
	strcpy(item->key, key);
	item->data = 0;
	table->n++;


	// Item insertion
	if (table->list[index] == NULL)
		table->list[index] = item;
	else
	{
		item->next = table->list[index];
		table->list[index] = item;
	}

	return item;
}
