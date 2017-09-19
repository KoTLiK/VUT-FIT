/**
 * @file       processes.c
 * @brief      Riesenie IOS - projekt2
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 * 
 */

#include "rollercoaster.h"

extern shared_memmory *shm;
extern sem_t *sem;
extern int shm_id;


void save_print(const char *action, int id, const char *name)
{
	sem_wait(&sem[PRINT]);
	fprintf(shm->fp, "%d\t: %s %d\t: %s\n", ++(shm->line), name, id, action);
	fflush(shm->fp);
	sem_post(&sem[PRINT]);
}


void ordered_print(unsigned order, const char *action, int id, const char *name)
{
	sem_wait(&sem[PRINT]);
	fprintf(shm->fp, "%d\t: %s %d\t: %s %d\n", ++(shm->line), name, id, action, order);
	fflush(shm->fp);
	sem_post(&sem[PRINT]);
}


void passenger(my_args_t *A, int id)
{
	char *action;
	save_print("started", id, "P");

	// Boarding
	sem_wait(&sem[BOARD]);
	save_print("board", id, "P");

	sem_wait(&sem[ORDER]);
		shm->count++;
		if (shm->count == A->C)
		{
			action = "board last";
			save_print(action, id, "P");
			shm->count = 0;
			sem_post(&sem[UNLOAD]);
		}
		else
		{
			action = "board order";
			ordered_print(shm->count, action, id, "P");
		}
	sem_post(&sem[ORDER]);

	// Unboarding
	sem_wait(&sem[UNBOARD]);
	save_print("unboard", id, "P");

	sem_wait(&sem[ORDER]);
		shm->count++;
		if (shm->count == A->C)
		{
			action = "unboard last";
			save_print(action, id, "P");
			shm->count = 0;
			sem_post(&sem[LOAD]);
		}
		else
		{
			action = "unboard order";
			ordered_print(shm->count, action, id, "P");
		}
	sem_post(&sem[ORDER]);

	// Finishing
	sem_wait(&sem[END]);
	save_print("finished", id, "P");
}


void car(my_args_t *A, int id)
{
	save_print("started", id, "C");

	unsigned i, j = 0;
	for (; j < (A->P / A->C); j++)
	{
		// Boarding
		save_print("load", id, "C");
		for (i = 0; i < A->C; i++)
			sem_post(&sem[BOARD]);

		sem_wait(&sem[UNLOAD]);
		// RUN
		save_print("run", id, "C");
		usleep(random_ms(A->rTime));

		// Unboarding
		save_print("unload", id, "C");
		for (i = 0; i < A->C; i++)
			sem_post(&sem[UNBOARD]);

		// Wait for the next run
		sem_wait(&sem[LOAD]);
	}

	// Finishing
	save_print("finished", id, "C");
	for (i = 0; i < A->P; i++)
		sem_post(&sem[END]);
}
