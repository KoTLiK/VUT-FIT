/**
 * @file       rollercoaster.h
 * @brief      Riesenie IOS - projekt2
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: gcc 4.8
 * 
 */

#ifndef ROLLERCOASTER_H_INCLUDED
#define ROLLERCOASTER_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

/**
 * @addtogroup Sctructures
 * @{
 */

	/**
	 * @brief      Arguments of program
	 * 
	 * @param      P      Processes (Passengers)
	 * @param      C      Car capacity
	 * @param      pTime  Max period in [ms] for new passenger
	 * @param      rTime  Max period in [ms] for one round of car
	 */
	typedef struct my_args {
		unsigned P;
		unsigned C;
		unsigned pTime;
		unsigned rTime;
	} my_args_t;

	/**
	 * @brief      Shared memmory
	 * 
	 * @param      count  Counter
	 * @param      fp     File pointer, output of program
	 */
	typedef struct {
		unsigned count;
		unsigned line;
		FILE *fp;
	} shared_memmory;
/** @} */

// For better indexing semaphores
enum semaphores {ORDER, PRINT, LOAD, UNLOAD, BOARD, UNBOARD, END};
// Number of semaphores
#define numSEM 7
// Size of all semaphores
#define sizeSEM ( sizeof(sem_t) * numSEM)

/**
 * @addtogroup Arguments and Warnings
 * @{
 */

	/**
	 * @brief      Checks if arguments are alright
	 *
	 * @param      A     Struct of arguments
	 * @param[in]  argc  The argc
	 * @param      argv  The argv
	 *
	 * @return     Zero if OK, else 1 (error occurs)
	 */
	int arguments_check(my_args_t *A, int argc, char const *argv[]);

	/**
	 * @brief      Converting argument into number with true value
	 *
	 * @param      argv   Argument
	 * @param      value  The value
	 * @param[in]  boo    The boo
	 *
	 * @return     Zero if OK, else 1 (error occurs)
	 */
	int a_checking(const char *argv, unsigned *value, int boo);

	/**
	 * @brief      Prints to stderr ERROR: "fmt"
	 *     same as printf format
	 *
	 * @param      fmt   Message format
	 */
	void warning_msg(const char *fmt, ...);
/** @} */

/**
 * @addtogroup Inicialization_resources
 * @{
 */

	/**
	 * @brief      Destroy all semaphores
	 *     In error call exit(2)
	 */
	void semaphores_destroy();

	/**
	 * @brief      Free all initialized memmory (shared memmory and semaphores)
	 */
	void all_free();

	/**
	 * @brief      Initialize shared memmory
	 *     In error call exit(2)
	 */
	void memmory_init();

	/**
	 * @brief      Initialize semaphores
	 *     In error call exit(2)
	 */
	void semaphores_init();
/** @} */

/**
 * @addtogroup Processes
 * @{
 */

	/**
	 * @brief      Print output message
	 *
	 * @param      action  The action of process
	 * @param      id      ID of process
	 * @param      name    The name of process
	 */
	void save_print(const char *action, int id, const char *name);

	/**
	 * @brief      Print output message
	 *
	 * @param      order   The order
	 * @param      action  The action of process
	 * @param      id      ID of process
	 * @param      name    The name of process
	 */
	void ordered_print(unsigned order, const char *action, int id, const char *name);

	/**
	 * @brief      I am a passenger
	 *
	 * @param      A     Parameters of program
	 * @param      id    ID of process
	 */
	void passenger(my_args_t *A, int id);

	/**
	 * @brief      I am a car
	 *
	 * @param      A     Parameters of program
	 * @param      id    ID of process
	 */
	void car(my_args_t *A, int id);
/** @} */

/**
 * @addtogroup Forks
 * @{
 */

	/**
	 * @brief      Calculate random number in interval from zero to [value]
	 *
	 * @param      value  Maximum value in interval
	 *
	 * @return     Value for usleep() function
	 */
	int random_ms(unsigned value);

	/**
	 * @brief      Forks new passengers
	 *
	 * @param      A     A storage of data for forking
	 */
	void fork_passengers(my_args_t *A);

	/**
	 * @brief      Suicide; call exit(2)
	 */
	void termination();
/** @} */

#endif