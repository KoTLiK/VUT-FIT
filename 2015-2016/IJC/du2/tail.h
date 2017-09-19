/**
 * @file       tail.h
 * @brief      Riesenie IJC-DU2-1, 11.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Ze zadaného vstupního souboru vytiskne posledních  10 řádků.
 *     Není-li zadán vstupní soubor, čte ze stdin. Je-li programu
 *     zadán parametr -n číslo, bude se tisknout tolik posledních
 *     řádků, kolik je zadáno parametrem 'číslo'.
 */

#ifndef TAIL_H_INCLUDED
#define TAIL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <limits.h>

/// Buffer limit of 510 characters and +1 null byte
#define B_LIM 511

/**
 * @brief      Checking if parameters are correct
 *
 * @param      N      (Number of lines) Will be changed if syntax is correct
 * @param      argv   Arguments of program (parameters)
 *
 * @return     Returns 0, if no problem occurs, else returns 1
 */
int set_num(int *N, char const *argv[]);

/**
 * @brief      Reads lines from stdin/FILE
 *
 * @param[in]  num       Number of lines
 * @param[in]  f_status  File status -> stdin or FILE
 * @param      filename  Filename from argv e.g. argv[f_status]
 *
 * @return     Indicates success (0) or not (1)
 */
int read_tail(int num, int f_status, char const *filename);

/**
 * @brief      Allocs sprace for lines
 *
 * @param      tail  Array of pointers
 * @param[in]  num   Number of lines
 *
 * @return     When error occurs NULL is returned, otherwise pointer at tail
 */
char *init_tail(char *tail[], int num);

/**
 * @brief      Makes free at allocated space
 *
 * @param      tail  Array of pointers
 * @param[in]  num   Number of lines
 */
void free_tail(char *tail[], int num);

/**
 * @brief      Rotates pointers in array by 1
 *
 * @param      tail  Array of pointers
 * @param[in]  num   Number of lines
 * @param[in]  boo   Direction of rotation
 */
void rot_tail(char *tail[], int num, int boo);

/**
 * @brief      Prints lines
 *
 * @param      tail  Array of pointers
 * @param[in]  i     Number of lines
 */
void print_tail(char *tail[], int i);

#endif
