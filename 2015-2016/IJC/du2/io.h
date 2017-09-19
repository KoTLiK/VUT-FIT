/**
 * @file       io.h
 * @brief      Riesenie IJC-DU2-2, 19.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Cte jedno slovo ze souboru f do zadaneho pole znaku
 *     a vrati delku slova (z delsich slov nacte prvnich max-1 znaku,
 *     a zbytek preskoci). Funkce vraci EOF, pokud je konec souboru.
 */

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdio.h>
#include <ctype.h>

/**
 * @brief      Get the word.
 *
 * @param      s     "String" buffer
 * @param[in]  max   Lenght limit
 * @param      f     Stream
 *
 * @return     Returns loaded word lenght,
 *             when end of file occurs the EOF constant is returned,
 *             when invalid stream pointer occurs -2 is returned
 *             and when invalid limit value occurs -3 is returned.
 */
int get_word(char *s, int max, FILE *f);

#endif
