/**
 * @file       tail2.h
 * @brief      Riesenie IJC-DU2-1, 12.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: g++ 4.8
 *     Ze zadaného vstupního souboru vytiskne posledních  10 řádků.
 *     Není-li zadán vstupní soubor, čte ze stdin. Je-li programu
 *     zadán parametr -n číslo, bude se tisknout tolik posledních
 *     řádků, kolik je zadáno parametrem 'číslo'.
 */

#ifndef TAIL2_H_INCLUDED
#define TAIL2_H_INCLUDED

#include <queue>
#include <string>
#include <iostream>
#include <fstream>

#include <climits>

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
 * @brief      Reads lines from stdin or FILE and only [num] last lines are printed
 *
 * @param[in]  num       Number of lines
 * @param[in]  f_status  File status -> stdin or FILE
 * @param      filename  Filename from argv e.g. argv[f_status]
 *
 * @return     Indicates success (0) or not (1)
 */
int read_tail(int num, int f_status, char const *filename);

#endif
