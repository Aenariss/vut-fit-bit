 

/**/
/*
 *  Pøedmìt: Algoritmy (IAL) - FIT VUT v Brnì
 *  Pokroèilé testy pro pøíklad c206.c (Dvousmìrnì vázaný lineární seznam)
 *  Vytvoøil: Martin Tuèek, záøí 2005
 *  Úprava: Bohuslav Køena, øíjen 2006
 */

int showFirstLast=0;	//Pri zmene na 1 ukazuje prvni a posledni prvek pole 



#include "c206.h"
                                                         /* pracovní promìnné */
tDLList TEMPLIST;
int obsah=1;
int MaxListLength = 100;                     /* Handles wrongly linked lists. */

/*******************************************************************************
 * Pomocné funkce usnadòující testování vlastní implementace.
 ******************************************************************************/

void print_elements_of_list(tDLList SEZNAM)	{

	tDLList temp_S=SEZNAM;
	int CurrListLength = 0;
	printf("-----------------");
	while ((temp_S.First!=NULL)&&(CurrListLength<MaxListLength))	{
		printf("\n \t%d",temp_S.First->data);
		if ((temp_S.First==SEZNAM.Act) && (SEZNAM.Act!=NULL))
			printf("\t <= toto je aktivní prvek ");
		temp_S.First=temp_S.First->rptr;
		CurrListLength++;
	}
    if (CurrListLength>=MaxListLength)
        printf("\nList exceeded maximum length!");
	printf("\n-----------------\n");
}

int test_DLInitList()	{
	solved=TRUE;
	DLInitList(&TEMPLIST);
	if (!solved)	{
		printf("Operace InitList nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLDisposeList()	{
	solved=TRUE;
	DLDisposeList(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLDisposeList nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLInsertFirst()	{
	solved=TRUE;
	DLInsertFirst(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLInsertFirst nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLInsertLast()	{
	solved=TRUE;
	DLInsertLast(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLInsertLast nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLFirst()	{
	solved=TRUE;
	DLFirst(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLFirst nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLLast()	{
	solved=TRUE;
	DLLast(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLLast nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLCopyFirst()	{
	solved=TRUE;
    int temp;
    DLCopyFirst(&TEMPLIST,&temp);
	if (!solved)	{
		printf("Operace DLCopyFirst nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace DLCopyFirst volala funkci DLError.\n");
			errflg=FALSE;
			return(FALSE);
		}
		else	{
			printf("Operace DLCopyFirst vrací obsah %d.\n",temp);
			return(TRUE);
		}
	}
}

int test_DLCopyLast()	{
	solved=TRUE;
    int temp;
    DLCopyLast(&TEMPLIST,&temp);
	if (!solved)	{
		printf("Operace DLCopyLast nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace DLCopyLast volala funkci DLError.\n");
			errflg=FALSE;
			return(FALSE);
		}
		else	{
			printf("Operace DLCopyLast vrací obsah %d.\n",temp);
			return(TRUE);
		}
	}
}

int test_DLDeleteFirst ()	{
	solved=TRUE;
	DLDeleteFirst(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLDeleteFirst nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLDeleteLast ()	{
	solved=TRUE;
	DLDeleteLast(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLDeleteLast nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPostDelete()	{
	solved=TRUE;
	DLPostDelete(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLPostDelete nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPreDelete()	{
	solved=TRUE;
	DLPreDelete(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLPreDelete nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPostInsert()	{
	solved=TRUE;
	DLPostInsert(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLPostInsert nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPreInsert()	{
	solved=TRUE;
	DLPreInsert(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLPreInsert nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLCopy()	{
	solved=TRUE;
	int temp;
	DLCopy(&TEMPLIST,&temp);

	if (!solved)	{
		printf("Operace DLCopy nebyla implementována!\n");
		return(FALSE);
	}
	else {
		if (errflg)	{
			printf("Operace DLCopy volala funkci DLError.\n");
			errflg=FALSE;
			return(FALSE);
		}
		else	{
			printf("Operace DLCopy vrací obsah %d.\n",temp);
			return(TRUE);
		}
	}
}

int test_DLActualize()	{
	solved=TRUE;
	DLActualize(&TEMPLIST,obsah);
	if (!solved)	{
		printf("Operace DLActualize nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLSucc()	{
	solved=TRUE;
	DLSucc(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLSucc nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLPred()	{
	solved=TRUE;
	DLPred(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLPred nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		print_elements_of_list(TEMPLIST);
		return(TRUE);
	}
}

int test_DLActive()	{
	solved=TRUE;
	int tmp=DLActive(&TEMPLIST);
	if (!solved)	{
		printf("Operace DLActive nebyla implementována!\n");
		return(FALSE);
	}
	else	{
		if (tmp)
			printf("Operace DLActive vrací hodnotu true.\n");
		else
			printf("Operace DLActive vrací hodnotu false.\n");
		return(TRUE);
	}
}

/*******************************************************************************
 * POKROÈILÉ TESTY
 * ---------------
 * Nejsou dostupné studentùm pøi øe¹ení domácích úloh.
 * Za jejich úspì¹né projítí získá student druhou èást bodù za pøíklad.
 *
 ******************************************************************************/

int main(int argc, char *argv[])	{
    printf("Dvousmìrnì vázaný lineární seznam\n");
    printf("=================================\n");

    printf("\n[TEST01]\n");
    printf("Inicializace seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    test_DLInitList();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                     /* Testy 02 a 03 jsou zde navíc oproti základním testùm. */
    printf("\n[TEST02]\n");
    printf("Pokus o volání DLCopyFirst na prázdný seznam => chyba\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopyFirst();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST03]\n");
    printf("Pokus o volání DLCopyLast na prázdný seznam => chyba\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopyLast();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST04]\n");
    printf("Zavoláme 2x operaci DLInsertFirst a 2x operaci DLInsertLast.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=1;
    test_DLInsertFirst();
    obsah=2;
    test_DLInsertFirst();
    obsah=3;
    test_DLInsertLast();
    obsah=4;
    test_DLInsertLast();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                             /* Test 05 je zde navíc oproti základním testùm. */
    printf("\n[TEST05]\n");
    printf("Seznam by mìl být neaktivní -- ovìøíme si to voláním DLActive.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                      /* Testy 06 a¾ 10 odpovídají základním testùm 03 a¾ 07. */
    printf("\n[TEST06]\n");
    printf("Otestujeme funkci DLFirst pøi neaktivním seznamu a funkci DLActive.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST07]\n");
    printf("Otestujeme funkci DLLast.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLLast();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST08]\n");
    printf("Test funkce DLCopy pøi aktivitì na posledním prvku\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopy();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST09]\n");
    printf("Test funkce DLPred -- voláme 3x, aktivita zùstane na prvním prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPred();
    test_DLPred();
    test_DLPred();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST10]\n");
    printf("Aktualizujeme obsah aktivního prvku.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=5;
    test_DLActualize();
    test_DLActive();
    test_DLCopy();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                     /* Základní testy 08 a¾ 10 zde nahradíme testem 11 a 12. */
    printf("\n[TEST11]\n");
    printf("Provedeme je¹tì jednou DLPred -- aktivita se ztratí.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPred();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST12]\n");
    printf("Nastavíme aktivitu na konec a pak ji zru¹íme operací DLSucc.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLLast();
    test_DLSucc();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                                                     /* Test 13 je zde navíc. */
    printf("\n[TEST13]\n");
    printf("Pokusíme se o aktualizaci pøi neaktivním seznamu => nic \n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=6;
    test_DLActualize();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                                     /* Test 14 odpovídá základnímu testu 11. */
    printf("\n[TEST14]\n");
    printf("Pokus o DLCopy pøi neaktivním seznamu => o¹etøená chyba.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopy();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                                             /* Testy 15 a 16 jsou zde navíc. */
    printf("\n[TEST15]\n");
    printf("DLSucc pøi neaktivním seznamu nesmí zhavarovat.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLSucc();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST16]\n");
    printf("DLPred pøi neaktivním seznamu nesmí zhavarovat.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPred();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                      /* Testy 17 a¾ 20 odpovídají základním testùm 12 a¾ 15. */
    printf("\n[TEST17]\n");
    printf("Pou¾ití operace DLCopyFirst pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopyFirst();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST18]\n");
    printf("Pou¾ití operace DLCopyLast pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopyLast();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST19]\n");
    printf("Pou¾ití operace DLDeleteFirst pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDeleteFirst();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST20]\n");
    printf("Pou¾ití operace DLDeleteLast pøi neaktivním seznamu\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDeleteLast();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                        /* Dále se ji¾ od základních testù výraznì odchýlíme. */
    printf("\n[TEST21]\n");
    printf("Operace DLFirst nastaví aktivitu na první prvek.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST22]\n");
    printf("DLDeleteFirst aktivního prvku povede ke ztrátì aktivity.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDeleteFirst();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST23]\n");
    printf("Operace DLLast nastaví aktivitu na poslední prvek.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLLast();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST24]\n");
    printf("Operace DLFirst pøi jediném prvku => aktivita zùstává.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST25]\n");
    printf("DLDeleteLast aktivního prvku povede ke ztrátì aktivity.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDeleteLast();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
           /* Teï máme prázdný seznam, tak si pro dal¹í testy vytvoøíme nový. */
    printf("\n[TEST26]\n");
    printf("Voláme 2x operaci DLInsertLast a 1x operaci DLInsertFirst.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah = 7;
    test_DLInsertLast();
    obsah = 8;
    test_DLInsertLast();
    obsah = 9;
    test_DLInsertFirst();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST27]\n");
    printf("Seznam je neaktivní -- ovìøíme si to voláním DLActive.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST28]\n");
    printf("U¾itím operace DLLast nastavíme aktivitu na konec seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLLast();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);
                                     /* Test 29 odpovídá základnímu testu 16. */
    printf("\n[TEST29]\n");
    printf("Operace DLPostDelete pøi aktivitì na posledním prvku nedìlá nic.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPostDelete();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST30]\n");
    printf("Nastavíme aktivitu na pøedposlední prvek operací DLPred a opìt\n");
    printf("zavoláme DLPostDelete. Sma¾eme tím poslední prvek v seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPred();
    test_DLPostDelete();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST31]\n");
    printf("U¾itím operace DLFirst nastavíme aktivitu na zaèátek seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST32]\n");
    printf("Operace DLPreDelete pøi aktivitì na prvním prvku nedìlá nic.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPreDelete();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST33]\n");
    printf("Nastavíme aktivitu na druhý prvek operací DLSucc a opìt\n");
    printf("zavoláme DLPreDelete. Sma¾e první prvek v seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLSucc();
    test_DLPreDelete();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST34]\n");
    printf("Otestujeme DLInsertFirst pøi seznamu s jediným prvkem, nastavíme\n");
    printf("aktivitu na novì vlo¾ený prvek a opìt posuneme aktivitu na konec \n");
    printf("seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=10;
    test_DLInsertFirst();
    test_DLFirst();
    test_DLSucc();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST35]\n");
    printf("Operací DLPostInsert vlo¾íme nový prvek za poslední prvek seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=11;
    test_DLPostInsert();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST36]\n");
    printf("Nastavíme aktivitu na první prvek seznamu a vyzkou¹íme DLPostInsert.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    obsah=12;
    test_DLPostInsert();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST37]\n");
    printf("Nastvíme aktivitu na druhý prvek a voláme 2x DLPostDelete a 1x DLPreDelete.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLSucc();
    test_DLPostDelete();
    test_DLPostDelete();
    test_DLPreDelete();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST38]\n");
    printf("Otestujeme DLInsertLast pøi seznamu s jediným prvkem, nastavíme\n");
    printf("aktivitu na novì vlo¾ený prvek a posuneme aktivitu na zaèátek.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=13;
    test_DLInsertLast();
    test_DLLast();
    test_DLPred();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST39]\n");
    printf("Operací DLPreInsert vkládáme nový prvek pøed první prvek seznamu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=14;
    test_DLPreInsert();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST40]\n");
    printf("Nastavíme aktivitu na poslední prvek seznamu a aplikujeme DLPreInsert.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLLast();
    obsah=15;
    test_DLPreInsert();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST41]\n");
    printf("Otestujeme funkènost operace DLDisposeList.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDisposeList();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

                                   /* Testování operací pøi prázdném seznamu. */
    printf("\nNásleduje testování operací pøi prázdném seznamu\n");
    printf("================================================\n");

    printf("\n[TEST42]\n");
    printf("DLFirst a DLLast pøi prázdném seznamu nedìlají nic.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLFirst();
    test_DLLast();
    test_DLActive();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST43]\n");
    printf("DLCopyFirst i DLCopyLast zpùsobí o¹etøenou chybu.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopyFirst();
    test_DLCopyLast();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST44]\n");
    printf("DLDeleteFirst a DLDeleteLast pøi prázdném seznamu nedìlají nic.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDeleteFirst();
    test_DLDeleteLast();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST45]\n");
    printf("DLPostDelete a DLPreDelete pøi prázdném seznamu nic nedìlají.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLPostDelete();
    test_DLPreDelete();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST46]\n");
    printf("DLPostInsert i DLPreInsert pøi prázdném seznamu nic nedìlají.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=16;
    test_DLPostInsert();
    obsah=17;
    test_DLPreInsert();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST47]\n");
    printf("Pokus o DLCopy na prázdný seznam => chyba\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopy();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST48]\n");
    printf("DLActualize pøi prázdném seznamu nic neudìlá.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    obsah=18;
    test_DLActualize();

    if(showFirstLast)
        printf("\nFirst: %d, Last: %d\n",TEMPLIST.First->data,TEMPLIST.Last->data);

    printf("\n[TEST49]\n");
    printf("DLSucc a DLPred na prázdný seznam nemají vliv.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLSucc();
    test_DLPred();
    test_DLActive();
    test_DLDisposeList();

    printf("\n----------------------- konec pøíkladu c206----------------------\n");

	return(0);
}
/**/
