/**
 * @file       steg-decode.c
 * @brief      Riesenie IJC-DU1, priklad b), 15.3.
 * @author     Milan Augustin
 * @details    Prelozene: gcc 4.9
 *     Ve funkci main načte ze souboru zadaného jako jediný argument
 *     programu obrázek ve formátu PPM a v něm najde uloženou
 *     "tajnou" zprávu pomocí prvočísel. Zprávu vytiskne na stdout.
 */

#include "ppm.h"
#include "error.h"
#include "eratosthenes.h"
#include <ctype.h>


int load_msg(char const *filename)
{
	struct ppm *image_data = NULL;
	if ((image_data = ppm_read(filename)) == NULL) return -1;

	char character[1] = {33};
	int b, flag = 0, b_pos = 0;
	unsigned long i, size = image_data->xsize * image_data->xsize * 3;

	ba_create(bit_pole, MAX+1);
	Eratosthenes(bit_pole);

	for (i = 2; i <= size; i++)
	{
		if (ba_get_bit(bit_pole, i) == 0)
		{
			b = (image_data->data[i] & 1) ? 1 : 0;
			DU1_SET_BIT_(character, b_pos, b); //character[0] = b ? (character[0] | (1 << b_pos)) : (character[0] & ~(1 << b_pos));

			b_pos++;
			if (b_pos == CHAR_BIT)
			{
				if (character[0] == '\0')
				{
					flag = 1;
					break;
				}

				if (!isprint(character[0]))
				{
					ppm_free(image_data);
					fatal_error("Sprava obsahuje netlacitelny znak.\n");
				}

				fprintf(stdout, "%c", character[0]);
				b_pos = 0;
			}
		}
	}

	ppm_free(image_data);

	if (flag == 0) fatal_error("V sprave chyba koncovy znak \\0 zo suboru %s\n", filename);
	
	fprintf(stdout, "\n");
	return 0;
}


int main(int argc, char const *argv[])
{
	if (argc < 2)
		fatal_error("Spustaj: ./%s du1-obrazek.ppm\n", argv[0]);

	if (load_msg(argv[1]) < 0) return -1;

	return 0;
}