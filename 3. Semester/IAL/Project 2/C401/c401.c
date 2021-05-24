
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/

	*RootPtr = NULL;	


}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/

	// Jestli aktualni uzel neni prazdny, vyhledavej
	if (RootPtr != NULL) {
		// Jestli je klic aktualniho uzlu shodny s hledanym, nasli jsme ho
		if (RootPtr->Key == K) {
			*Content = RootPtr->BSTNodeCont;
			return TRUE;
		}
		// Jestli je aktualni klic mensi nez hledany klic, rekurzivne sestupujeme do praveho podstromu
		else if (RootPtr->Key < K) {
			return BSTSearch(RootPtr->RPtr, K, Content);
		}
		// Jestli je aktualni klic vetsi nez hledany klic, rekurzivne sestupujeme do leveho podstromu
		else if (RootPtr->Key > K) {
			return BSTSearch(RootPtr->LPtr, K, Content);
		}
	}
	//Jestli jsme se nikde jinde nevratili, uzel neexistuje
	return FALSE;
}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/

	// Jestli je koren prazdny, vytvorime novy s danym obsahem
	if (*RootPtr == NULL) {
		*RootPtr = malloc(sizeof(struct tBSTNode));
		// Jestli se malloc nepovedl, vratime se 
		if (*RootPtr == NULL) {
			return;
		}
		(*RootPtr)->Key = K;
		(*RootPtr)->BSTNodeCont = Content;
		(*RootPtr)->LPtr = NULL;
		(*RootPtr)->RPtr = NULL;
	}
	else {
        // Jestli se dany klic rovna klici akalnimu klici, prepiseme uzel
        if (K == (*RootPtr)->Key) {
			(*RootPtr)->BSTNodeCont = Content;
		}
        // Jestli je dany klic vetsu nez aktualni klic, rekurzivne sestupujeme doprava
		else if ((*RootPtr)->Key < K) {
			BSTInsert(&(*RootPtr)->RPtr, K, Content);
		}
		// Jestli je dany klic mensi nez aktualni klic, rekurzivne sestupujeme doleva
		else if ((*RootPtr)->Key > K) {
			BSTInsert(&(*RootPtr)->LPtr, K, Content);
		}
	}

}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně.
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete().
**/

	// Zkontroluj, jestli rootptr a ptrreplaced jsou validni
	if (*RootPtr == NULL || PtrReplaced == NULL) { 
		return;
	}
	else {
		// Zkontroluj, jestli existuje pravy podstrom 
		if ((*RootPtr)->RPtr != NULL) {
			// Jestli ano, rekurzivne se do nej presun
			ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
		}
		else {
			// Jinak bude nejpravejsi uzel prave rootptr
			PtrReplaced->Key = (*RootPtr)->Key;
			PtrReplaced->BSTNodeCont = (*RootPtr)->BSTNodeCont;
			// Zachovej hodnotu LPtr puvodniho uzlu
			tBSTNodePtr left = (*RootPtr)->LPtr;
			// Uvolni puvodni
			free(*RootPtr);
			// Nahrad jeho puvodni pozici uzlem z LPtr
			*RootPtr = left;
		}
	}
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/

	if (*RootPtr == NULL) {
		return;
	} 
	else {
		// Jestli je aktualni klic mensi nez hledany klic, rekurzivne sestupujeme do praveho podstromu
		if ((*RootPtr)->Key < K) {
			BSTDelete(&(*RootPtr)->RPtr, K);
		}
		// Jestli je aktualni klic vetsi nez hledany klic, rekurzivne sestupujeme do leveho podstromu
		else if ((*RootPtr)->Key > K) {
			BSTDelete(&(*RootPtr)->LPtr, K);
		}
		// Jestli je klic shodny, muzeme zacit mazat
		else if ((*RootPtr)->Key == K) {

			// Mazany uzel ma jen levy podstrom
			if (((*RootPtr)->LPtr != NULL) && (((*RootPtr)->RPtr == NULL))) {
				tBSTNodePtr replace = (*RootPtr)->LPtr;
				free(*RootPtr);
				*RootPtr = replace;
			}

			// Mazany uzel ma jen pravy podstrom
			else if (((*RootPtr)->LPtr == NULL) && (((*RootPtr)->RPtr != NULL))) {
				tBSTNodePtr replace = (*RootPtr)->RPtr;
				free(*RootPtr);
				*RootPtr = replace;
			}
			// Mazany uzel ma oba podstromy
			else if (((*RootPtr)->LPtr != NULL) && (((*RootPtr)->RPtr != NULL))) {
					ReplaceByRightmost(*RootPtr, &(*RootPtr)->LPtr);
			}
			// Mazany uzel nema zadny podstrom
			else {
				free(*RootPtr);
				*RootPtr = NULL;
			}

		}
	}
	


}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
	
	// Jestli je rootptr prazdny, neni co rusit
	if (*RootPtr == NULL) {
		return;
	}

	else {
		// Rekurzivne sestup do leveho podstromu
		BSTDispose(&(*RootPtr)->LPtr);
		// Rekurzivne sestup do praveho podstromu
		BSTDispose(&(*RootPtr)->RPtr);
		// Uvolni prave zpracovavany uzel
		free(*RootPtr);
		// Nastav uzel do puvodniho stavu
		*RootPtr = NULL;
	}
}
/* konec c401.c */

