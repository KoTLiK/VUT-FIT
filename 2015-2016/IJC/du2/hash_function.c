/**
 * @file       hash_function.c
 * @brief      Riesenie IJC-DU2-2, 19.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Rozptylovacia funkcia na urcenie indexu do tabulky
 */

unsigned int hash_function(const char *str, unsigned htab_size)
{
	unsigned int h = 0;
	const unsigned char *p;
	for (p = (const unsigned char *)str; *p != '\0'; p++)
		h = 65599 * h + *p;
	return h % htab_size;
}
