/**
 * @file       ppm.h
 * @brief      Riesenie IJC-DU1, priklad b), 15.3.2016
 * @author     Milan Augustin
 * @details    Prelozene: gcc 4.9
 *     Nacita obsah suboru vo formate .ppm do struktury alebo
 *     Zapise obsah struktury do suboru vo formate .ppm
 */

#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED

#include "error.h"
#include <string.h>

#define MAX 5000*5000*3

struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[];	// RGB bajty, celkem 3*xsize*ysize
};

void ppm_free(struct ppm *p);

struct ppm * ppm_read(const char * filename);

int ppm_write(struct ppm *p, const char * filename);

#endif