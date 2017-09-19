/**
 * @file       arguments.c
 * @brief      Riesenie IOS - projekt2
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 * 
 */

#include "rollercoaster.h"


int arguments_check(my_args_t *A, int argc, char const *argv[])
{
	if (argc != 5)
	{
		warning_msg("Nedostatok argumentov.\n");
		return 1;
	}

	int flag = 0, err[4] = {0, };

	// rTime 4. argument chyba
	err[3] = a_checking(argv[4], &(A->rTime), 0);
	// pTime 3. argument chyba
	err[2] = a_checking(argv[3], &(A->pTime), 0);
	// Car 2. argument chyba
	err[1] = a_checking(argv[2], &(A->C), -1);
	// Passenger 1. argument chyba
	err[0] = a_checking(argv[1], &(A->P), A->C);

	for (int i = 0; i < 4; i++)
		if (err[i])
		{
			warning_msg("Chybny %d. argument.\n", i+1);
			flag = 1;
		}

	return flag;
}


int a_checking(const char *argv, unsigned *value, int boo)
{
	if (strlen(argv) > 9) return 1;

	char *endptr = NULL;
	int num = strtol(argv, &endptr, 10);
	if (*endptr != '\0') return 1;

	if (boo) // Passenger or Car
	{
		if (num <= 0) return 1;
		if (boo > 0) // Passenger
		{
			if (num <= boo) return 1;
			if ((num % boo) != 0) return 1;
		}
	} // else -> only Time
	else if (num < 0 || num > 5000) return 1;

	*value = (unsigned) num;
	return 0;
}


void warning_msg(const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "ERROR: ");
	vfprintf(stderr, fmt, va);
	va_end(va);
}
