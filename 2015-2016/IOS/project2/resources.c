/**
 * @file       resources.c
 * @brief      Riesenie IOS - projekt2
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 * 
 */

#include "rollercoaster.h"

extern shared_memmory *shm;
extern sem_t *sem;
extern int shm_id;


void all_free()
{
	if (shm != NULL)
	{
		if (shm->fp != NULL)
		{
			fclose(shm->fp); // Closing File
			shm->fp = NULL;
		}

		// Unmapping shared memmory
		shmdt(shm);
		shmctl(shm_id, IPC_RMID, NULL);

		shm = NULL;
	}

	if (sem != NULL) // Removing semaphores
		semaphores_destroy();
}


void semaphores_destroy()
{
	for (int i = 0; i < numSEM; i++)
		sem_destroy(&sem[i]); // Bye bye semaphore

	munmap(sem, sizeSEM); // Unmapping semaphores

	sem = NULL;
}


void memmory_init()
{
	key_t smh_key = ftok("./rollercoaster.c", 1);

	// Mapping shared memmory
	if ((shm_id = shmget(smh_key, sizeof(shared_memmory), IPC_CREAT | IPC_EXCL | 0666)) == -1)
	{
		warning_msg("Problem pri vytvarani zdielanej pamete\n");
		semaphores_destroy();
		exit(2);
	}

	// Ataching shared memmory
	if ((shm = (shared_memmory *) shmat(shm_id, NULL, 0)) == (void *) -1)
	{
		warning_msg("Problem pri pristupe k zdielanej pameti\n");
		
		shmctl(shm_id, IPC_RMID, NULL);
		semaphores_destroy();
		exit(2);
	}

	// Inicialization of shared memmory
	shm->count = 0;
	shm->line = 0;

	if ((shm->fp = fopen("proj2.out", "w")) == NULL)
	{ // Error occurs -> all_free
		shmdt(shm);
		shmctl(shm_id, IPC_RMID, NULL);
		semaphores_destroy();
		shm = NULL;
		exit(2);
	}
}


void semaphores_init()
{
	int i, err_sem = 0, init_sem[numSEM] = {0, };

	// Mapping semaphores
	if ((sem = mmap(NULL, sizeSEM, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
	{
		warning_msg("Problem pri mapovani pamete\n");
		exit(2);
	}

	// Inicialization first 2 semaphores to opened
	for (i = 0; i < LOAD; i++)
		if (sem_init(&sem[i], 1, 1) == -1) err_sem = 1;
		else init_sem[i] = 1;

	// Inicialization rest of semaphores to closed
	for (; i < numSEM; i++)
		if (sem_init(&sem[i], 1, 0) == -1) err_sem = 1;
		else init_sem[i] = 1;

	// Handle ERRORS
	if (err_sem)
	{
		for (i = 0; i < numSEM; i++)
			if (init_sem[i])
				sem_destroy(&sem[i]);

		munmap(sem, sizeSEM);
		sem = NULL;
		exit(2);
	}
}
