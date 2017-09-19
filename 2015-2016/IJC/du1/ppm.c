/**
 * @file       ppm.c
 * @brief      Riesenie IJC-DU1, priklad b), 15.3.2016
 * @author     Milan Augustin
 * @details    Prelozene: gcc 4.9
 *     Nacita obsah suboru vo formate .ppm do struktury alebo
 *     Zapise obsah struktury do suboru vo formate .ppm
 */

#include "ppm.h"

void ppm_free(struct ppm *p)
{
	if (p != NULL) free(p);
}

struct ppm * ppm_read(const char * filename)
{
	FILE *fr = NULL;
	struct ppm *p = NULL;
	unsigned x, y;
	
	if ( (fr = fopen(filename, "r")) == NULL)
	{
		warning_msg("Problem pri otvarani suboru %s .\n", filename);
		return NULL;
	}


	if (fscanf(fr, "P6 %u %u 255\n", &x, &y) != 2)
	{
		warning_msg("Zlyhalo nacitanie dat zo suboru %s .\n", filename);
		goto END_ER_R;
	}


	if (x*y*3 > MAX)
	{
		warning_msg("Prekrocenie limitu 5000*5000*3\n");
		goto END_ER_R;
	}


	if ( (p = malloc( sizeof(struct ppm) + MAX )) == NULL)
	{
		warning_msg("Zlyhanie alokacie pamete.\n");
		goto END_ER_R;
	}


	p->xsize = x;
	p->ysize = y;

	if (fread(p->data, 1, p->xsize * p->ysize * 3, fr) != (p->xsize * p->ysize * 3))
	{
		warning_msg("Nacitanie dat RGB zo suboru %s stroskotalo.\n", filename);
		ppm_free(p);
		goto END_ER_R;
	}


	if (fclose(fr) != 0)
		warning_msg("Chyba pri zatvarani suboru %s .\n", filename);
	return p;

	END_ER_R:
		if (fclose(fr) != 0)
			warning_msg("Chyba pri zatvarani suboru %s .\n", filename);
		return NULL;
}

int ppm_write(struct ppm *p, const char * filename)
{
	FILE *fw;
	if ( (fw = fopen(filename, "wb")) == NULL)
	{
		warning_msg("Problem otvarania suboru %s .\n", filename);
		return -1;
	}

	if (fprintf(fw, "P6\n%u %u\n255\n", p->xsize, p->ysize) < 0)
		goto END_ER_W;

	if (fwrite(p->data, 1, p->xsize * p->ysize * 3, fw) != (p->xsize * p->ysize * 3))
		goto END_ER_W;
	
	
	if (fclose(fw) != 0)
		warning_msg("Nepodaril sa zapis do suboru %s .\n", filename);
	return 0;

	END_ER_W:
		if (fclose(fw) != 0)
			warning_msg("Nepodaril sa zapis do suboru %s .\n", filename);
		return -1;
}