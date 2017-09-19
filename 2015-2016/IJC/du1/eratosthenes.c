/**
 * @file       eratosthenes.c
 * @brief      Riesenie IJC-DU1, priklad a), 4.3.2016
 * @author     Milan Augustin, xaugus09
 * @details    Prelozene: gcc 4.9
 *     Implementacia algoritmu: Eratostenovo sito, bity s hodnotou 0 su prvocisla
 */

#include "eratosthenes.h"

void Eratosthenes(bit_array_t pole) {
	unsigned long idx, N, size = ba_size(pole);
	unsigned long sqr = sqrt(size);
	ba_set_bit(pole, 0, 1);
	ba_set_bit(pole, 1, 1);
	for (idx = 2; idx <= sqr; idx++)
		if (ba_get_bit(pole, idx) == 0)
			for (N = 2; N*idx < size; N++)
				ba_set_bit(pole, N*idx, 1);
}