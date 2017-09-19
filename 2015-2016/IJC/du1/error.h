/**
 * @file       error.h
 * @brief      Riesenie IJC-DU1, priklad b), 5.3.2016
 * @author     Milan Augustin
 * @details    Prelozene: gcc 4.9
 *     Implementovanie funkci na chybove hlasenia, tlacia text "CHYBA: "
 *     a potom chybove hlasenie podla formatu fmt
 *     fatal_error() ukoncuje program volanim exit(1)
 */

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...);

void fatal_error(const char *fmt, ...);

#endif