/*
>>>     subor:          proj1.c
>>>     autor:          Milan Augustín, xaugus09@stud.fit.vutbr.cz
>>>     projekt, rok:   IZP - Projekt 1 (Praca s textom), 2015
>>>
>>>     popis programu:
>>>         Vstupnym textom je standardny vstup (stdin). Vstupny subor sa zpracováva po slovach.
>>>         Program bude detekovat ci nacitane slovo je cislo (pripadne prvocislo), datum (vo formate DDDD-DD-DD)
>>>         alebo ci ide o slovo (pripadne palindrom).
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int dlzka_slova (char *str);
int over_cislo (char *str);
int prime (char *str);
int over_datum (char *str);
int date (char *str);
int over_palindrom (char *str);
int over_slovo (char *str);

int main(int argc, char *argv[])    /* |||||||||||||||||||||* ZACIATOK *||||||||||||||||||||| */
{
    (void) argv;
    if (argc > 1)
    {
        printf("Error (Terminated):\n  Tento program sa spusta iba s jednym parametrom,\n  bohuzial bol spusteny s viacerymi parametrami.\n");
        return 1;
    }
    char str[102];
    int code;
    while ((code = scanf("%101s", str)) != EOF)
    {
        if (code == 0)
        {
            printf("Error (Terminated):\n  Chyba vstupu!\n");
            return 1;
        }
        if (dlzka_slova(str) > 100)
        {
            printf("Error (Terminated):\n  Neplatny vstup! Prekroceny limit 100 znakov.\n");
            return 1;
        }
        if (over_cislo(str) == -1)
        {
            if (over_datum(str))
            {
                printf("Date: ");
                date(str);
                continue;
            }
            else
                if (over_slovo(str))
                {
                    if (over_palindrom(str))
                        printf("Word: %s (Palindrom)\n", str);
                    else
                        printf("Word: %s\n", str);
                }
                else
                {
                    printf("Error (Terminated):\n  Neplatny znak!\n");
                    return 1;
                }
        }
        else
            if (over_cislo(str))
                printf("Number: %s (Prime)\n", str);
            else
                printf("Number: %s\n", str);
    }
    return 0;
}   /* ||||||||||||||||||||||||||||||||||||||* KONIEC *|||||||||||||||||||||||||||||||||||||| */

int dlzka_slova (char *str) // pocita dlzku slova
{
    int dlzka = 0;
    while (str[dlzka] != '\0')
        dlzka++;
    return dlzka;
}

int over_cislo (char *str)  // overuje ci string je cislo a zaroven overuje, ci cislo je aj prvocislom
    // return == '-1' (slovo); '0' (cislo); '1' (prvocislo)
{
    int i = 0;
    while (isdigit(str[i]))
        i++;    // zistuje ci sa nachadzaju cifry v stringu
    if (i == dlzka_slova(str))
    {
        while (str[0] == '0')   // pokial je nulty znak nula, odstrani sa
            if (dlzka_slova(str) > 1)   // ak su vsetky cifry nuly, ostane iba 1 nula
                for (i = 0; i < dlzka_slova(str); i++)
                    str[i] = str[i + 1];
            else
                break;
        if (dlzka_slova(str) <= 10 && atoll(str) <= INT_MAX)
            // pokial je to cislo a dlzka cisla je <= 10, zabranim preteceniu pri LLINT a mozem bezpecne previest string na cislo
            // a zaroven zistim ci to ma zmysel prevadzat na INT
            return prime(str);
        else return 0;
    }
    else return -1;
}

int prime (char *str) // zistuje ci ide o prvocislo
{   // return == '0' (number); '1' (prime)
    int cislo = atoi(str);
    if (cislo <= 1) return 0;
    if (cislo == 2) return 1;
    for (long long int i = 2; (i*i) <= cislo; i++)
        if (cislo % i == 0) return 0;
    return 1;
}

int over_datum (char *str) // overuje ci string je vo formate DDDD-DD-DD
{   // return == '1' (success); '0' (nezodpoveda formatu)
    if (dlzka_slova(str) == 10 && str[4] == '-' && str[7] == '-')   // pusti len format XXXX-XX-XX
    {
        char r[6], m[4], d[3];
        sscanf(str, "%5s %3s %2s", r, m, d);    // string si rozdelim na mensie stringy
        for (int i = 0; isdigit(r[i]); i++) // overi ci prvy string je cislo
            if (i == 3)
                for (int j = 0; isdigit(m[j]); j++) // overi druhy
                    if (j == 1)
                        for (int k = 0; isdigit(d[k]); k++) // overi treti
                            if (k == 1)
                            {
                                int year, month, day;
                                sscanf(str, "%d-%d-%d", &year, &month, &day);
                                if (year > 1901 && (month >=1 && month <=12) && (day >=1 && day <=31))
                                    return 1;   // splnuje rozsah datumu
                            }
        return 0;
    }
    return 0;
}

int date (char *str) // format datumu spracuje a vypluje aj s dnom
{
    struct tm time_str;
    char daybuf[20];
    int y, m, d;
    sscanf(str, "%d-%d-%d", &y, &m, &d);
    time_str.tm_year = y - 1900;
    time_str.tm_mon = m - 1;
    time_str.tm_mday = d;
    time_str.tm_hour = 0;
    time_str.tm_min = 0;
    time_str.tm_sec = 1;
    time_str.tm_isdst = -1;
    if (mktime(&time_str) == -1)
        puts("-unknown-");
    else
    {
        strftime(daybuf, sizeof(daybuf), "%a %F", &time_str);
        puts(daybuf);
    }
    return 0;
}

int over_palindrom (char *str)  // overuje string (slovo) ci je palindrom... return == '0' (slovo); '1' (palindrom)
{
    for (int i = 1; i <= (dlzka_slova(str)/2); )    // 'i' funguje ako posuvnik indexu
        if (str[i - 1] == str[dlzka_slova(str) - i])
            i++;
        else return 0;
    return 1;
}

int over_slovo (char *str) // overuje ci nacitane znaky su platne
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (isprint(str[i]))
            i++;
        else
            return 0;
    }
    return 1;
}