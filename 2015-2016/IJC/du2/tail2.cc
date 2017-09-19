/**
 * @file       tail2.cc
 * @brief      Riesenie IJC-DU2-1, 12.4.2016
 * @author     Milan Augustin, xaugus09, VUT-FIT
 * @details    Prelozene: g++ 4.8
 *     Ze zadaného vstupního souboru vytiskne posledních  10 řádků.
 *     Není-li zadán vstupní soubor, čte ze stdin. Je-li programu
 *     zadán parametr -n číslo, bude se tisknout tolik posledních
 *     řádků, kolik je zadáno parametrem 'číslo'.
 */

#include "tail2.h"

/* ================================= START ================================= */
int main(int argc, char const *argv[])
{
	std::ios::sync_with_stdio(false);
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
			{
				std::cerr << "Nespravne zadane argumenty\n" << "   Pouzitie: " <<
					argv[0] << " [-n NUM] [FILE]\n" << "   vsetky argumenty su volitelne\n";
				return 1;
			}
			if (argc == 4) file_status = 3; // [FILE] occurs
		}
		else // wrong syntax for this program
		{
			std::cerr << "Nespravne zadane argumenty\n" << "   Pouzitie: " <<
				argv[0] << " [-n NUM] [FILE]\n" << "   vsetky argumenty su volitelne\n";
			return 1;
		}


	if (!N) return N; // N == 0 => I don't need any reading


	if (read_tail(N, file_status, argv[file_status]))
		return 1;


	return 0;
}
/* ================================== END ================================== */


int set_num(int *N, char const *argv[])
{
	if ((std::string)argv[1] != "-n") return 1;

	std::string snum = (std::string)argv[2];
	if (snum.size() > 10) return 1;

	std::string::size_type endptr = 0;
	long long int num = std::stoll(snum,&endptr);
	if (!endptr) return 1;
	if (num > INT_MAX || num < 0) return 1;

	*N = (int) num;

	return 0;
}


int read_tail(int num, int f_status, char const *filename)
{
	std::queue<std::string> myqueue;
	std::string line;
	int count = 0;

	if (f_status)
	{
		std::ifstream myfile (filename); // Opening file
		if (!myfile)
		{
			std::cerr << "Problem pri otvarani suboru "
				<< filename << '\n';
			return 1;
		}

		while (std::getline(myfile, line))
		{
			myqueue.push(line);
			if (count == num) // Counting number of loaded lines
				myqueue.pop(); // Deleting useless line
			else
				count++;
		}

		myfile.close();
	}
	else
		while (std::getline(std::cin, line))
		{
			myqueue.push(line);
			if (count == num) // Counting number of loaded lines
				myqueue.pop(); // Deleting useless line (oldest)
			else
				count++;
		}


	// Result printing
	while(!myqueue.empty())
	{
		std::cout << myqueue.front() << '\n';
		myqueue.pop();
	}

	return 0;
}
