/*
>>>     subor:          proj2.c
>>>     autor:          Milan Augustín, xaugus09@stud.fit.vutbr.cz
>>>     projekt, rok:   IZP - Projekt 2 (Iteracne vypocty), 2015
>>>
>>>     popis programu:
>>>			Program pocita prirodzeny logaritmus pomocou binarnych operacii +, -, *, /. Dalej program dokaze
>>>			vyhladat pozadovany pocet iteracii na pozadovanu presnost z hodnot zadaneho intervalu.
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))
// ABSolutna hodnota z vyrazu 'x'

int vstup(int argc, char *argv[]);
int vstup_log(char *argv[]);
int vstup_iter(char *argv[]);
void print_log(char *argv[]);
void print_iter(char *argv[]);
double taylor_log(double x, unsigned int n);
double taylor_iter(double min, double max, double eps);
double bisekcia_taylor(double x, double eps);
double cfrac_log(double x, unsigned int n);
double cfrac_iter(double min, double max, double eps);
double bisekcia_cfrac(double x, double eps);

int main(int argc, char *argv[])	/* |||||||||||||||||||||* ZACIATOK *||||||||||||||||||||| */
{
	switch(vstup(argc, argv))
	{
		case -1:	// neplatny vstup
		{
			fprintf(stderr,
					">>> Neplatny vstup pre %s !\n>>>\n"
					">>>   Zadaj: %s --log X N\n"
					">>>     pricom X je hodnota pre logaritmus a N pre pocet iteracii, pre N > 0\n>>>\n"
					">>>   Alebo zadaj %s --iter MIN MAX EPS\n"
					">>>     pricom MIN a MAX je interval kde MIN < MAX\n"
					">>>     a EPS >= 0 je presnost, ktorou sa ma pocitat\n", argv[0], argv[0], argv[0]);
			return 42;	// pred ukoncenim vypise chybovu hlasku s malou napovedou
		}
		case 1:	// vypis/vystup pre vstup --log X N
		{
			print_log(argv);
			break;
		}
		case 2:	// vypis/vystup pre vstup --iter MIN MAX EPS
		{
			print_iter(argv);
			break;
		}
	}
	return 0;
}	/* ||||||||||||||||||||||||||||||||||||||* KONIEC *|||||||||||||||||||||||||||||||||||||| */

/* taylorov rad tvaru: log(1 - x)= -sum(x^n / n) a log(x)= sum( ((x - 1)/x) ^n)/n ...... '^' znak pre mocninu, nie XOR */
double taylor_log(double x, unsigned int n)
{
	if (x <= 0)	// urcenie definicneho oboru pre logaritmus
	{
		if (x == 0)
			return -INFINITY;	// pre log(0) == -nekonecno
		return NAN;	// pre log(-x) == nedefinovane
	}
	if (isinf(x))	// nebude predsa pocitat s nekonecnom, rovno vypise ze ide o nekonecno
		return INFINITY;
	double sum = 0;
	double p = 1.0;
	if (x < 1)	// medzna hodnota pre 2 vzorce
	{
		x = 1.0 - x;	// podmienka pre tuto implementaciu vzorca
		for (unsigned int i = 1; i <= n; i++)	// vzorec pre interval 0 < x < 2
		{
			p *= x;	// do 'p' sa uklada postnupne sa zvysujuca mocnina vyrazu
			if (sum == (sum - (p / i)) )	// niekedy nastane bod ked sa bude robit rozdiel s tak malym cislom ze sa 'sum' uz nezmeni == ukoncenie
				return sum;
			sum -= (p / i);	// suma vsetkych clenov radu
		}
	}
	else
		for (unsigned int i = 1; i <= n; i++)	// vzorec pre interval 1/2 < x
		{
			p *= ((x - 1) / x);	// do 'p' sa uklada postnupne sa zvysujuca mocnina vyrazu
			if (sum == (sum + (p / i)) )	// niekedy nastane bod ked sa bude robit rozdiel s tak malym cislom ze sa 'sum' uz nezmeni == ukoncenie
				return sum;
			sum += (p / i);	// suma vsetkych clenov radu
		}
	return sum;
}

/* porovnava hodnotu rozdielu log(min alebo max) - taylor_log(min alebo max, n) s presnostou, vysledkom je pocet iteracii potrebnych na vypocet so zadanou presnostou*/
double taylor_iter(double min, double max, double eps)
{
	if (min < 0 && max < 0)	// pokial su vstupne hodnoty mimo definicny obor, je zbytocne hovorit o potrebnom pocte iteracii
		return 0;
	double iter_1 = bisekcia_taylor(min, eps);
	double iter_2 = bisekcia_taylor(max, eps);
	if (iter_1 < 0)	// ak nahodou bisekcia pretecie (unsigned int) aby sa o tom dozvedel program
		return (-1);
	if (iter_2 < 0)	// ak nahodou bisekcia pretecie (unsigned int) aby sa o tom dozvedel program
		return (-1);
	//unsigned int iter_1 = 0;
	//unsigned int iter_2 = 0;
	//double num;
	/* vseobecne uvazujeme ze nepozname skutocnu hodnotu, ktoru hladame, tak preto musime porovnavat rozdiel (n-1 clenom a n clenom) s presnostou
	do
	{
		iter_1++;
		num = taylor_log(min, iter_1 - 1) - taylor_log(min, iter_1);
		if (num == 0)
			break;
	} while (ABS(num) > eps);
	do
	{
		iter_2++;
		num = taylor_log(max, iter_2 - 1) - taylor_log(max, iter_2);
		if (num == 0)
			break;
	} while (ABS(num) > eps);
	return (iter_1 > iter_2 ? iter_1 : iter_2);
	*/
	/*
	do
	{
		if (iter_1 == UINT_MAX)
			break;
		iter_1++;	// prvy vstup == prva inkrementacia == prva iteracia
		num = log(min) - taylor_log(min, iter_1);
		if (num == 0)	// bezpecnostny if, keby nahodou sa to chcelo zacyklit (nastalo to v pripade rozdielu s predchadzajucou iteraciou)
			break;
	} while (ABS(num) > eps);
	do
	{
		if (iter_2 == UINT_MAX)
			break;
		iter_2++;	// prvy vstup == prva inkrementacia == prva iteracia
		num = log(max) - taylor_log(max, iter_2);
		if (num == 0)	// vid vyssie
			break;
	} while (ABS(num) > eps);
	*/
	return (iter_1 > iter_2 ? iter_1 : iter_2);	// vyssi pocet iteracii vyhrava, pretoze chceme vediet potrebny pocet iteracii pre cely interval
}

/* vyhladavanie poctu iteracii pomocou bisekcie pre taylor_log */
double bisekcia_taylor(double x, double eps)
{
	unsigned long long int a = 1;
	unsigned long long int b = 1;
	double num;
	while (1)	// podmienky su nastavene tak, aby nekonecny cyklus skoncil v kazdom pripade
	{
		num = log(x) - taylor_log(x,b);
		if (ABS(num) > eps)	// tu posuvam interval smerom k max (unsigned int) a popri tom posuvam min, aby nebol prilis velky interval
		{
			if ((2 * b) > UINT_MAX)	// tu sa kontroluje ci nahodou nepreteka unsigned int
			{
				b = UINT_MAX;	// ak pretiekol, tu sa nastavi na UINT_MAX
				num = log(x) - taylor_log(x,b);
				if (ABS(num) > eps)	// pokial UINT_MAX iteracii nevyhovuje presnosti, returnem '-1' == znamenie o preteceni
					return (-1);
			}
			a = b;
			b *= 2;	// posuvam sa podla mocniny 2ky
		}
		else
		{
			while (1)	// podmienky su nastavene tak, aby nekonecny cyklus skoncil v kazdom pripade
			{
				num = log(x) - taylor_log(x, ((a + b) /2) );
				if (ABS(num) > eps)	// tu orezavam vytvoreny interval na mensi a mensi
				{
					if ( ((a + b) /2) == a)	// ak presnost nevyhovuje posunieme sa v intervale smerom nahor, tento if kontroluje ci nejde uz o koniec intervalu == nemam sa kam posunut
					{
						num = log(x) - taylor_log(x,a);
						if (ABS(num) > eps)	// podla vyhodnotenia urcim ktore cele cislo z intervalo 2 cisel je vyhladany pocet iteracii
							return b;
						else
							return a;
					}
					a = (a + b) /2;
				}
				else
				{
					b = (a + b) /2;
					if ( ((a + b) /2) == a)	// pokial novo posunuty interval ma 2 cisla, dalej uz posuvat nebudem interval a budem vyhodnocovat
					{
						num = log(x) - taylor_log(x,a);
						if (ABS(num) > eps)	// podla vyhodnotenia urcim ktore cele cislo z intervalo 2 cisel je vyhladany pocet iteracii
							return b;
						else
							return a;
					}
				}
			}
		}
	}
}

/* pocitanie logaritmu pomocou zretazeneho zlomku */
double cfrac_log(double x, unsigned int n)
{
	if (x <= 0)	// urcenie definicneho oboru pre logaritmus
	{
		if (x == 0)
			return -INFINITY;	// pre log(0) == -nekonecno
		return NAN;	// pre log(-x) == nedefinovane
	}
	if (isinf(x))	// nebude predsa pocitat s nekonecnom, rovno vypise ze ide o nekonecno
		return INFINITY;
	if (n == 0)
		return 0.0;
	x = (x - 1)/(x + 1);	// uprava podmienky pre zretazeny zlomok
	n -= 1;	// uprava poctu iteracii, kedze v poslednej uprave (priradeniu) 'cfrac' prebieha jedna iteracia navyse
	double cfrac = 0.0;
	double p = x*x;	// nahrada 2. mocniny cisla x
	double i = n;	// nahrada uint za double kvoli preteceniu
	unsigned long long int k = 2 * n + 1;	// pocita neparne cisla, pouzivaju sa vo vzorci (nahrada)
	for ( ; n > 0; n--, k -= 2, i--)	// vypocet od posledneho zlomku, nahor
		cfrac = (-(i * i * p) / (k + cfrac));
	cfrac = (2 * x) / (1 + cfrac);
	return cfrac;
}

/* porovnava hodnotu rozdielu log(min alebo max) - cfrac_log(min alebo max, n) s presnostou, vysledkom je pocet iteracii potrebnych na vypocet so zadanou presnostou*/
double cfrac_iter(double min, double max, double eps)
{
	if (min < 0 && max < 0)	// pokial su vstupne hodnoty mimo definicny obor, je zbytocne hovorit o potrebnom pocte iteracii
		return 0;
	double iter_1 = bisekcia_cfrac(min, eps);
	double iter_2 = bisekcia_cfrac(max, eps);
	if (iter_1 < 0)	// ak nahodou bisekcia pretecie (unsigned int) aby sa o tom dozvedel program
		return (-1);
	if (iter_2 < 0)	// ak nahodou bisekcia pretecie (unsigned int) aby sa o tom dozvedel program
		return (-1);
	//unsigned int iter_1 = 0;
	//unsigned int iter_2 = 0;
	//double num;
	/* vseobecne uvazujeme ze nepozname skutocnu hodnotu, ktoru hladame, tak preto musime porovnavat rozdiel (n-1 clenom a n clenom) s presnostou
	do
	{
		iter_1++;
		num = cfrac_log(min, iter_1 - 1) - cfrac_log(min, iter_1);
		if (num == 0)
			break;
	} while (ABS(num) > eps);
	do
	{
		iter_2++;
		num = cfrac_log(max, iter_2 - 1) - cfrac_log(max, iter_2);
		if (num == 0)
			break;
	} while (ABS(num) > eps);
	return (iter_1 > iter_2 ? iter_1 : iter_2);
	*/
	/*
	do
	{
		if (iter_1 == UINT_MAX)
			break;
		iter_1++;	// prvy vstup == prva inkrementacia == prva iteracia
		num = log(min) - cfrac_log(min, iter_1);
		if (num == 0)	// vid v 'taylor_iter()'
			break;
	} while (ABS(num) > eps);
	do
	{
		if (iter_1 == UINT_MAX)
			break;
		iter_2++;	// prvy vstup == prva inkrementacia == prva iteracia
		num = log(max) - cfrac_log(max, iter_2);
		if (num == 0)	// vid v 'taylor_iter()'
			break;
	} while (ABS(num) > eps);
	*/
	return (iter_1 > iter_2 ? iter_1 : iter_2);	// vyssi pocet iteracii vyhrava, pretoze chceme vediet potrebny pocet iteracii pre cely interval
}

/* vyhladavanie poctu iteracii pomocou bisekcie pre cfrac_log */
double bisekcia_cfrac(double x, double eps)
{
	unsigned long long int a = 1;
	unsigned long long int b = 1;
	double num;
	while (1)	// podmienky su nastavene tak, aby nekonecny cyklus skoncil v kazdom pripade
	{
		num = log(x) - cfrac_log(x,b);
		if (ABS(num) > eps)	// tu posuvam interval smerom k max (unsigned int) a popri tom posuvam min, aby nebol prilis velky interval
		{
			if ((2 * b) > UINT_MAX)	// tu sa kontroluje ci nahodou nepreteka unsigned int
			{
				b = UINT_MAX;	// ak pretiekol, tu sa nastavi na UINT_MAX
				num = log(x) - cfrac_log(x,b);
				if (ABS(num) > eps)	// pokial UINT_MAX iteracii nevyhovuje presnosti, returnem '-1' == znamenie o preteceni
					return (-1);
			}
			a = b;
			b *= 2;	// posuvam sa podla mocniny 2ky
		}
		else
		{
			while (1)	// podmienky su nastavene tak, aby nekonecny cyklus skoncil v kazdom pripade
			{
				num = log(x) - cfrac_log(x, ((a + b) /2) );
				if (ABS(num) > eps)	// tu orezavam vytvoreny interval na mensi a mensi
				{
					if ( ((a + b) /2) == a)	// ak presnost nevyhovuje posunieme sa v intervale smerom nahor, tento if kontroluje ci nejde uz o koniec intervalu == nemam sa kam posunut
					{
						num = log(x) - cfrac_log(x,a);
						if (ABS(num) > eps)	// podla vyhodnotenia urcim ktore cele cislo z intervalo 2 cisel je vyhladany pocet iteracii
							return b;
						else
							return a;
					}
					a = (a + b) /2;
				}
				else
				{
					b = (a + b) /2;
					if ( ((a + b) /2) == a)	// pokial novo posunuty interval ma 2 cisla, dalej uz posuvat nebudem interval a budem vyhodnocovat
					{
						num = log(x) - cfrac_log(x,a);
						if (ABS(num) > eps)	// podla vyhodnotenia urcim ktore cele cislo z intervalo 2 cisel je vyhladany pocet iteracii
							return b;
						else
							return a;
					}
				}
			}
		}
	}
}

/* ocistenie vstupu */
int vstup(int argc, char *argv[])
{
	if (argc >= 4)
	{
		if (strcmp(argv[1], "--log") == 0)	// --log X N ...skontroluje tento vstup
			return vstup_log(argv);
		if (strcmp(argv[1], "--iter") == 0)	// --iter MIN MAX EPS ...skontroluje tento vstup
			return vstup_iter(argv);
		return -1;
	}
	return -1;
}

/* kontroluje zadane hodnoty X N z argv[] ci su spravne a ci su v spravnom formate */
int vstup_log(char *argv[])
{
	char *endptr = NULL;
	strtod(argv[2], &endptr);	// kontrola pre argv[2] ktory zodpoveda premennej 'x' (hodnota pre X)
	if (*endptr != '\0')
		return -1;
	double n = strtod(argv[3], &endptr);	// kontrola pre argv[3] ktory zodpoveda premennej 'n' (pocet iteracii N, pre N > 0)
	if ((*endptr != '\0') || (n > UINT_MAX || n < 0))
		return -1;
	return 1;	// vstup prebehol vporiadku
}

/* kontroluje zadane hodnoty MIN MAX EPS ci z argv[] su spravne a ci su v spravnom formate */
int vstup_iter(char *argv[])
{
	char *endptr = NULL;
	double min = strtod(argv[2], &endptr);	// kontrola hodnoty MIN
		if (*endptr != '\0')
			return -1;
	double max = strtod(argv[3], &endptr);	// kontrola hodnoty MAX
		if (*endptr != '\0')
			return -1;
		if (isnan(min) || isnan(max))	// kontrola, ze su to realne hodnoty cisel
			return -1;
	double eps = strtod(argv[4], &endptr);	// kontrola hodnoty EPS
		if (*endptr != '\0')
			return -1;
		if (eps < 0 || isnan(eps) || isinf(eps))	// presnost EPS musi byt nezaporna (EPS >= 0) a zaroven, ze je to realne vycislitelna hodnota
			return -1;
	return 2;	// vstup prebehol vporiadku
}

/* vstupne podmienky boli splnene, mozem bez problemov nacitat hodnoty X a N z argumentov a vytlacit ich */
void print_log(char *argv[])
{
	char *endptr;	// najradsej by som to dal prec, asi bude vhodnejsie pouzit inu funkciu
	double x = strtod(argv[2], &endptr);
	double n = strtod(argv[3], &endptr);
	// v printe volam funkcie na vypocet hodnot logaritmov pomocou hodnot z X, N a formatovany vystup ich vytlaci podla zadanej predlohy postupne
	printf("       log(%g) = %.12g\n", x, log(x));
	printf(" cfrac_log(%g) = %.12g\n", x, cfrac_log(x, n));
	printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, n));
}

/* vstupne podmienky boli splnene, mozem bez problemov nacitat hodnoty MIN, MAX a EPS z argumentov a vytlacit ich */
void print_iter(char *argv[])
{
	char *endptr;	// rovnaky sposob nacitania hodnot ako v print_log()
	double min = strtod(argv[2], &endptr);
	double max = strtod(argv[3], &endptr);
	double eps = strtod(argv[4], &endptr);
	// vytlaci formatovany vystup podla predlohy s funkciou log z math.h
	printf(	"       log(%g) = %.12g\n"
			"       log(%g) = %.12g\n",
			min, log(min), max, log(max));

	// v printe volam funkcie na vypocet logaritmov pomocou hodnot MIN, MAX, iter_c, iter_t a formatovany vystup ich vytlaci podla zadanej predlohy postupne
	// pouzivam funkcie na vypocet log zo zadaneho poctu iteracii, ktore som si zistil pomocou funkcii *_iter a ulozil ich do premennych
	double iter_c = cfrac_iter(min, max, eps);	// inicializovana premenna pre uchovanie poctu iteracii, aby sa funkcia zavolala iba raz
	if (iter_c < 0)	// cfrac_iter moze vratit zapornu hodnotu '-1' co znamena ze unsigned int pretiekol
		printf("continued fraction iterations = iterations overflow\n");
	else
		printf(	"continued fraction iterations = %u\n"
				"    cf_log(%g) = %.12g\n"
				"    cf_log(%g) = %.12g\n",
				(unsigned int) iter_c, min, cfrac_log(min, iter_c), max, cfrac_log(max, iter_c));

	double iter_t = taylor_iter(min, max, eps);	// inicializovana premenna pre uchovanie poctu iteracii, aby sa funkcia zavolala iba raz
	if (iter_t < 0)	// cfrac_iter moze vratit zapornu hodnotu '-1' co znamena ze unsigned int pretiekol
		printf("taylor polynomial iterations = iterations overflow\n");
	else
		printf( "taylor polynomial iterations = %u\n"
				"taylor_log(%g) = %.12g\n"
				"taylor_log(%g) = %.12g\n",
				(unsigned int) iter_t, min, taylor_log(min, iter_t), max, taylor_log(max, iter_t));
}