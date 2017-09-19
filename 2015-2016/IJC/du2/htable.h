/**
 * @file       htable.h
 * @brief      Riesenie IJC-DU2-2, 20.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Rozhranie kniznice pre hashovaciu tabulku
 */

#ifndef HTABLE_H_INCLUDED
#define HTABLE_H_INCLUDED

/**
 * @addtogroup Sctructures
 * @{
 */

/**
 * @brief      Structure of hash table list item
 * 
 * @param      key    Pointer at dynamic allocated memmory
 * @param      data   Number of occurrences
 * @param      next   Pointer at next item
 */
typedef struct htab_listitem
{
	char *key;
	unsigned data;
	struct htab_listitem *next;
} htab_listitem_t;

/**
 * @brief      Structure of hash table
 * 
 * @param      htab_size       Size of array
 * @param      hash_fun_ptr()  Pointer at hash function
 * @param      n               Number of records
 * @param      list            Array of list items
 */
typedef struct htab
{
	unsigned htab_size;
	unsigned (*hash_fun_ptr)();
	unsigned n;
	struct htab_listitem *list[];
} htab_t;
/** @} */

/**
 * @brief      Rozptylovacia funkcia na urcenie indexu do tabulky
 *
 * @param[in]  str        Keyword
 * @param[in]  htab_size  Size of hash table
 *
 * @return     Index
 */
unsigned int hash_function(const char *str, unsigned htab_size);

/**
 * @brief      Pro vytvoreni a inicializaci tabulky
 *
 * @param[in]  size  Size of table
 *
 * @return     Pointer at initialized table
 *             When error occurs NULL pointer is returned
 */
htab_t * htab_init(unsigned size);

/**
 * @brief      Pro vytvoreni a inicializaci tabulky s jinou nez implicitni rozptylovaci funkci
 *
 * @param[in]  size    Size of table
 * @param[in]  hashfn  Pointer at hash function
 *
 * @return     Pointer at initialized table
 *             When error occurs NULL pointer is returned
 */
htab_t * htab_init2(unsigned size, unsigned int (*hashfn)(const char *, unsigned));

/**
 * @brief      Vyhleda zaznam odpovidajici retezci "key"
 *
 * @param      table  Pointer at hash table
 * @param      key    Keyword
 *
 * @return     Pokud jej nalezne, vrati ukazatel na zaznam
 *             Pokud nenalezne, automaticky prida zaznam a vrati ukazatel
 *             Pri chybe vraci NULL pointer
 */
htab_listitem_t * htab_lookup_add(htab_t *table, char *key);

/**
 * @brief      Volani funkce fn pro kazdy prvek
 *
 * @param      table  Pointer at hash table
 * @param[in]  fn     Pointer at function called for each element
 */
void htab_foreach(htab_t *table, void (*fn)(char *key, unsigned value));

/**
 * @brief      Vyhledani a zruseni zadane polozky
 *
 * @param      table  Pointer at hash table
 * @param      key    Keyword
 */
void htab_remove(htab_t *table, char *key);

/**
 * @brief      Zruseni vsech polozek v tabulce
 *
 * @param      table  Pointer at hash table
 */
void htab_clear(htab_t *table);

/**
 * @brief      Zruseni cele tabulky
 *
 * @param      table  Pointer at hash table
 */
void htab_free(htab_t *table);

/**
 * Moja lenivost vsetko prelozit
 */

#endif
