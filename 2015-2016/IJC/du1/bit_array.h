/**
 * @file       bit_array.h
 * @brief      Riesenie IJC-DU1, priklad a), 4.3.2016
 * @author     Milan Augustin, xaugus09
 * @details    Prelozene: gcc 4.9
 *     Implementovanie bitoveho pola makrami a inline funkciami
 */

#ifndef BIT_ARRAY_H_INCLUDED
#define BIT_ARRAY_H_INCLUDED

#include <limits.h>
#include "error.h"

typedef unsigned long bit_array_t[];

#define UL_BITS (CHAR_BIT * sizeof(unsigned long))
#define UL_POS(i) ((i / UL_BITS) +1)

#define ba_create(jmeno_pole, velikost) \
	unsigned long jmeno_pole[(UL_POS(velikost) + ((velikost % UL_BITS) != 0))] = { velikost, 0 }

#define T_BITS(p) (CHAR_BIT * sizeof(*p))
#define T_POS(p, i) (i / (sizeof(*p) * CHAR_BIT))

#define DU1_SET_BIT_(p, i, b) p[T_POS(p,i)] = (b) ? \
	(p[T_POS(p,i)] | (1ULL << (i % T_BITS(p)))) : \
	(p[T_POS(p,i)] & ~(1ULL << (i % T_BITS(p))))
#define DU1_GET_BIT_(p, i) (p[T_POS(p,i)] & (1ULL << (i % T_BITS(p)))) ? 1 : 0

/// INLINE FUNKCIE alebo MAKRA
	#ifndef USE_INLINE
	#define MAKRA
		#define ba_size(jmeno_pole) jmeno_pole[0]

		#define ba_set_bit(jmeno_pole, index, vyraz) \
			do { if (index < ba_size(jmeno_pole)) jmeno_pole[UL_POS(index)] = (vyraz) ? \
				(jmeno_pole[UL_POS(index)] | (1ULL << (index % UL_BITS))) : (jmeno_pole[UL_POS(index)] & ~(1ULL << (index % UL_BITS))); \
			else fatal_error("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)ba_size(jmeno_pole)); } while(0)

		#define ba_get_bit(jmeno_pole, index) \
			(index >= ba_size(jmeno_pole)) ? \
			(fatal_error("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)ba_size(jmeno_pole)), 0) : \
			(jmeno_pole[UL_POS(index)] & (1ULL << (index % UL_BITS)) ? 1 : 0)
	#else
		inline unsigned long ba_size(bit_array_t jmeno_pole) { return jmeno_pole[0]; }

		inline void ba_set_bit(bit_array_t jmeno_pole, unsigned long index, int vyraz) {
			if (index < ba_size(jmeno_pole)) jmeno_pole[UL_POS(index)] = (vyraz) ? \
				(jmeno_pole[UL_POS(index)] | (1ULL << (index % UL_BITS))) : (jmeno_pole[UL_POS(index)] & ~(1ULL << (index % UL_BITS)) );
			else fatal_error("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)ba_size(jmeno_pole));
		}

		inline int ba_get_bit(bit_array_t jmeno_pole, unsigned long index) {
			if (index >= ba_size(jmeno_pole))
				fatal_error("Index %ld mimo rozsah 0..%ld\n", (long)index, (long)ba_size(jmeno_pole));
			return (jmeno_pole[UL_POS(index)] & (1ULL << (index % UL_BITS)) ? 1 : 0);
		}
	#endif
#endif // BIT_ARRAY_H_INCLUDED
