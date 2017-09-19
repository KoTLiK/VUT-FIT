/**
 * @file       rollercoaster.c
 * @brief      Riesenie IOS - projekt2
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 * 
 */

#include "rollercoaster.h"

shared_memmory *shm = NULL;
sem_t *sem = NULL;
int shm_id;


int main(int argc, char const *argv[])
{
	my_args_t A;
	setbuf(stderr, NULL);

	if (arguments_check(&A, argc, argv)) return 1;

	signal(SIGTERM, termination);
	signal(SIGINT, termination);

	semaphores_init();
	memmory_init();

	srand(time(NULL));

	// --------------------------------------
	
	int pid;
	pid_t pids[2] = {0, 0};

	if ((pid = fork()) < 0) // Fork ERROR
	{
		warning_msg("Zomrel fork\n");
		termination();
	}

	if (pid == 0)  // Child
	{
		signal(SIGTERM, termination);
		signal(SIGINT, termination);

		car(&A, 1);

		fclose(shm->fp);
		exit(0);
	}
	else // Parent
	{
		pids[0] = pid;
		if ((pid = fork()) < 0) // Fork ERROR
		{
			warning_msg("Zomrel fork\n");
			all_free();
			kill(pids[0], SIGTERM);
			exit(2);
		}

		if (pid == 0) // Child
		{
			signal(SIGTERM, termination);
			signal(SIGINT, termination);

			fork_passengers(&A);

			fclose(shm->fp);
			exit(0);
		}
		else pids[1] = pid; // Parent
	}

	// Waiting for the end, LP rulez
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], NULL, 0);

	// --------------------------------------

	all_free();
	return 0;
}



int random_ms(unsigned value)
{ //	srand(time(NULL));
	if (!value) return 0;
	return ((rand() % (value+1)) * 1000);
}


void termination()
{
	all_free();
	kill(0, SIGTERM);
	exit(2);
}


void fork_passengers(my_args_t *A)
{
	pid_t pid[A->P];
	unsigned i;

	for (i = 0; i < A->P; i++)
	{
		usleep(random_ms(A->pTime));

		if ((pid[i] = fork()) == 0) // Child
		{
			signal(SIGTERM, termination);
			signal(SIGINT, termination);

			passenger(A, i+1);

			fclose(shm->fp);
			exit(0);
		}
		else if (pid[i] < 0) // Fork ERROR
		{
			warning_msg("Zomrel fork pre pasaziera %d\n", i+1);
			termination();
		}

		// Parent => continue
	}

	// Waiting for the end, LP rulez
	for (i = 0; i < A->P; i++)
		waitpid(pid[i], NULL, 0);
}
