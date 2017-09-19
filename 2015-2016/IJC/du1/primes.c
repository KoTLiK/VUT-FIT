/**
 * @file       primes.c
 * @brief      Riesenie IJC-DU1, priklad a), 7.3.2016
 * @author     Milan Augustin
 * @details    Prelozene: gcc 4.9
 *     Pomocou Eratosthenovho sita vypocita vsetky prvocisla po cislo 202,000,000
 *     po ukonceni funcie Erato... program vypise poslednych 10 prvocisel vzostupne
 */

#include "eratosthenes.h"
#define N 202000000

int main(void)
{
	ba_create(bp, N);
	Eratosthenes(bp);

	unsigned long p[10], size = ba_size(bp) -1;
	int count = 9;	// 0-9 indexy
	while (count >= 0)
	{
		if (ba_get_bit(bp, size) == 0) p[count--] = size;
		size--;
	}
	for (int i = 0; i < 10; i++) printf("%ld\n", p[i]);

	return 0;
}