/**
 * @file       tail.c
 * @brief      Riesenie IJC-DU2-1, 11.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 *     Ze zadaného vstupního souboru vytiskne posledních  10 řádků.
 *     Není-li zadán vstupní soubor, čte ze stdin. Je-li programu
 *     zadán parametr -n číslo, bude se tisknout tolik posledních
 *     řádků, kolik je zadáno parametrem 'číslo'.
 */

#include "tail.h"
#include "error.h"


/* ================================= START ================================= */
int main(int argc, char const *argv[])
{
	int N = 10; // default lines count value
	int file_status = 0;

	// default last 10 lines from STDIN or [FILE]
	if (argc <= 2)
	{
		if (argc == 2) file_status = 1; // [FILE] occurs
	}
	else if (argc < 5) // setup with parameter [-n NUMBER] from STDIN or [FILE]
		{
			if (set_num(&N, argv))
				fatal_error("Nespravne zadane argumenty\n"
					"   Pouzitie: %s [-n NUM] [FILE]\n"
					"   vsetky argumenty su volitelne\n", argv[0]);
			if (argc == 4) file_status = 3; // [FILE] occurs
		}
		else // wrong syntax for this program
			fatal_error("Nespravne zadane argumenty\n"
				"   Pouzitie: %s [-n NUM] [FILE]\n"
				"   vsetky argumenty su volitelne\n", argv[0]);


	if (!N) return N; // N == 0 => I don't need any reading


	if (read_tail(N, file_status, argv[file_status]))
		return 1;


	return 0;
}
/* ================================== END ================================== */


int set_num(int *N, char const *argv[])
{
	if (strcmp(argv[1], "-n") != 0) return 1;
	
	if (strlen(argv[2]) > 10) return 1;

	char *endptr = NULL;
	long long int num = strtoll(argv[2], &endptr, 10);
	if (*endptr != '\0') return 1;
	if (num > INT_MAX || num < 0) return 1;

	*N = (int) num;

	return 0;
}


int read_tail(int num, int f_status, char const *filename)
{
	FILE *stream = stdin;
	int line = num-1, flag = 1;
	char *tail[num], str[B_LIM];

	if (f_status) // Opening file
		if ( (stream = fopen(filename, "r")) == NULL)
		{
			warning_msg("Problem pri otvarani suboru %s\n", filename);
			return 1;
		}

	if (init_tail(tail, num) == NULL) // Allocating memmory
	{
		if (stream != stdin) // Closing file, if is not a stdin
			if (fclose(stream) != 0)
				warning_msg("Chyba pri zatvarani suboru %s .\n", filename);
		return 1;
	}


	while (fgets(tail[line], B_LIM, stream) != NULL)
	{
		if ((strlen(tail[line]) == (B_LIM - 1)) && (tail[line][B_LIM - 2] != '\n')) // Line seems to be longer than B_LIM-1 (510)
			while (fgets(str, B_LIM, stream) != NULL) // If is not end of file, read rest of line
			{
				if (flag)
				{
					flag = 0;
					warning_msg("Prekrocil sa limit %d znakov na riadok\n", (B_LIM-1));
				}
				if (strchr(str, '\n') != NULL) break; // Finding end of line
			}

		if (line != 0)
			line--;
		else
			rot_tail(tail, num, 1);
	}

	rot_tail(tail, num, 0);


	if (stream != stdin) // Closing file, if is not a stdin
		if (fclose(stream) != 0)
			warning_msg("Chyba pri zatvarani suboru %s .\n", filename);


	print_tail(tail, num); // Result printing


	free_tail(tail, num); // Safe ending ;-)
	return 0;
}


/// Functions for read_tail()

	char *init_tail(char *tail[], int num)
	{
		for (int i = 0; i < num; i++)
			if ((tail[i] = malloc(B_LIM)) == NULL)
			{
				free_tail(tail, i);
				warning_msg("Zlyhanie alokacie pamete.\n");
				return NULL;
			}

		return tail[0];
	}


	void free_tail(char *tail[], int num)
	{
		for (int i = 0; i < num; ++i)
			free(tail[i]);
	}


	void rot_tail(char *tail[], int num, int boo)
	{
		char *temp = NULL;
		int i;

		if (boo)
		{
			temp = tail[num-1];
			for (i = num-1; i > 0; i--)
				tail[i] = tail[i-1];

			tail[0] = temp;
		}
		else
		{
			temp = tail[0];
			for (i = 0; i < num-1; i++)
				tail[i] = tail[i+1];

			tail[num-1] = temp;
		}
	}


	void print_tail(char *tail[], int i)
	{
		for (i--; i >= 0; i--)
		{
			printf("%s", tail[i]);
			if ((strlen(tail[i]) == (B_LIM - 1)) && (tail[i][B_LIM - 2] != '\n'))
				putchar('\n');
		}
	}
