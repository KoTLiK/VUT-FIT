/**
 * @file       htab_init2.c
 * @brief      Riesenie IJC-DU2-2, 20.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Pro vytvoreni a inicializaci tabulky
 *     s jinou nez implicitni rozptylovaci funkci
 */

#include "htable.h"
#include <stdlib.h>

htab_t * htab_init2(unsigned size, unsigned int (*hashfn)(const char *, unsigned))
{
	htab_t *table;

	if ((table = calloc( 1, sizeof(htab_t) + (size * sizeof(struct htab_listitem *)) )) == NULL)
		return NULL;

	table->htab_size = size;
	table->hash_fun_ptr = hashfn;
	table->n = 0;

	return table;
}
