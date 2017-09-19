﻿	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

/**
 * Riesitel: xaugus09
 */

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
	return;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	tDLElemPtr del = NULL;
	while ((del = L->First) != NULL)
	{
		L->First = L->First->rptr;
		free(del);
	}

	L->Act = NULL;
	L->Last = NULL;
	return;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr new = NULL;
	if ((new = malloc(sizeof(struct tDLElem))) == NULL)
	{
		DLError();
		return;
	}

    new->data = val;
    new->lptr = NULL;
    new->rptr = L->First;

	if (L->First == NULL) // Osetrenie pre prvy zaznam vobec
		L->Last = new;
	else
    	new->rptr->lptr = new;

	L->First = new;
    return;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr new = NULL;
	if ((new = malloc(sizeof(struct tDLElem))) == NULL)
	{
		DLError();
		return;
	}

	new->data = val;
	new->rptr = NULL;
    new->lptr = L->Last;

	if (L->First == NULL) // Osetrenie pre prvy zaznam vobec
		L->First = new;
	else
    	new->lptr->rptr = new;

	L->Last = new;
    return;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
	return;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
	return;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->First == NULL)
		DLError();
	else
		*val = L->First->data;
	return;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->First == NULL)
		DLError();
	else
		*val = L->Last->data;
	return;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L->First != NULL)
	{
		if (L->First == L->Act)
			L->Act = NULL;
		if (L->First == L->Last)
			L->Last = NULL;

		tDLElemPtr del = L->First;
		L->First = del->rptr;

		if (L->First != NULL)
			L->First->lptr = NULL;

		free(del);
	}

	return;
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	if (L->First != NULL)
	{
		if (L->Last == L->Act)
			L->Act = NULL;
		if (L->First == L->Last)
			L->First = NULL;

		tDLElemPtr del = L->Last;
		L->Last = del->lptr;

		if (L->Last != NULL)
			L->Last->rptr = NULL;
		
		free(del);
	}

	return;
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if (L->Act == NULL || L->Act == L->Last) return;

	tDLElemPtr del = L->Act->rptr;
	if (del == L->Last) // ci ide o posledny prvok
		L->Last = L->Act; // Act prvok sa stane poslednym prvkom
	else // previazanie pravej strany zoznamu s aktivnym prvkom
		del->rptr->lptr = L->Act;

	// presunutie naslednika z prvku ktory sa odstrani na aktivny prvok
	L->Act->rptr = del->rptr;
	free(del);
	return;
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if (L->Act == NULL || L->Act == L->First) return;

	tDLElemPtr del = L->Act->lptr;
	if (del == L->First) // ci ide o prvy prvok
		L->First = L->Act; // Act prvok sa stane prvym prvkom
	else // previazanie lavej strany zoznamu s aktivnym prvkom
		del->lptr->rptr = L->Act;

	// presunutie naslednika z prvku ktory sa odstrani na aktivny prvok
	L->Act->lptr = del->lptr;
	free(del);
	return;
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act == NULL) return;
	tDLElemPtr new = NULL;
	if ((new = malloc(sizeof(struct tDLElem))) == NULL)
	{
		DLError();
		return;
	}

	new->data = val;
	new->rptr = L->Act->rptr;
	L->Act->rptr = new;
	new->lptr = L->Act;
	if (L->Act != L->Last) // rozdelenie middle/last
		new->rptr->lptr = new;
	else
		L->Last = new;
	return;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act == NULL) return;
	tDLElemPtr new = NULL;
	if ((new = malloc(sizeof(struct tDLElem))) == NULL)
	{
		DLError();
		return;
	}

	new->data = val;
	new->lptr = L->Act->lptr;
	L->Act->lptr = new;
	new->rptr = L->Act;
	if (L->Act != L->First) // rozdelenie middle/first
		new->lptr->rptr = new;
	else
		L->First = new;
	return;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if (L->Act == NULL)
		DLError();
	else
		*val = L->Act->data;
	return;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if (L->Act != NULL)
		L->Act->data = val;
	return;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->rptr;
	return;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->lptr;
	return;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/