/**
 * @file       error.c
 * @brief      Riesenie IJC-DU1, priklad b), 5.3.2016
 * @author     Milan Augustin
 * @details    Prelozene: gcc 4.9
 *     Implementovanie funkci na chybove hlasenia, tlacia text "CHYBA: "
 *     a potom chybove hlasenie podla formatu fmt
 *     fatal_error() ukoncuje program volanim exit(1)
 */

#include "error.h"


void warning_msg(const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "CHYBA: ");
	vfprintf(stderr, fmt, va);
	va_end(va);
}

void fatal_error(const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "CHYBA: ");
	vfprintf(stderr, fmt, va);
	va_end(va);
	exit(1);
}