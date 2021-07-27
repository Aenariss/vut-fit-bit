 
/**/
/****************************** c203-test.c ************************************
 * Pøedmìt: Algoritmy (IAL) - FIT VUT v Brnì
 * Pokroèilé testy pro pøíklad c203.c (Fronta znakù v poli)
 * Vytvoøil: Václav Topinka, záøí 2005
 * Upravil: Bohuslav Køena, øíjen 2006
 ******************************************************************************/

#include "c203.h"

#include <stdio.h>
#include <stdlib.h>
                                             /* variables used within testing */
tQueue* queue;
int QUEUE_SIZE;
int solved;
int err_flag;

/*******************************************************************************
 * The following functions allow us to write shorter testing code.
 * They usually call particular operation, check whether the function
 * has been implemented, and, eventualy, print a result of the function.
 ******************************************************************************/

void queuePrint ( tQueue* q ) {
/* Prints a queue with the following index meaning:
 * F = Front
 * B = Back
 * E = queue is Empty (F == B)
 */
	putchar('\n');
                                                   /* Prints a queue content. */
	for ( int i=0; i<QUEUE_SIZE; i++ )
		putchar(q->arr[i]);
	putchar('\n');
                                                 /* Prints a queue index(es). */
	for ( int i=0; i<QUEUE_SIZE; i++ ) {
		if ( ( i == q->f_index ) || ( i == q->b_index ) )
			putchar('^');
		else
			putchar(' ');
	}
	putchar('\n');
                                                 /* Prints index(es) meaning. */
	for ( int i=0; i<QUEUE_SIZE; i++ ) {
		if ( (i == q->f_index) && (i==q->b_index))
			putchar('E');
		else if ( i == q->f_index )
			putchar ('F');
		else if ( i == q->b_index )
			putchar ('B');
		else
			putchar (' ');
	}
	putchar('\n');
}

void use_queue_init ( tQueue* q ) {
    solved = 1;
	queueInit( q );
	if ( ! solved )
		printf("Function queueInit has not been implemented.\n");
}

void use_queue_empty ( tQueue* q ) {
    solved = 1;
	int i = queueEmpty( q );
	if ( ! solved )
		printf("Function queueEmpty has not been implemented.\n");
	else
		printf("Function queueEmpty returned '%s'.\n", i ? "TRUE" : "FALSE");
}

void use_queue_full ( tQueue* q ) {
	solved = 1;
	int i = queueFull( q );
	if ( ! solved )
		printf("Function queueFull has not been implemented.\n");
	else
		printf("Fucntion queueFull returned '%s'.\n", i ? "TRUE" : "FALSE");
}

void use_queue_front ( tQueue* q ) {
	solved = 1;
	err_flag = 0;
	char c;
	queueFront( q, &c );
	if ( ! solved )
		printf("Function queueFront has not been implemented.\n");
	else if ( !err_flag )
		printf("Function queueFront returned value '%c'.\n", c );
}

void use_queue_remove ( tQueue* q ) {
	solved = 1;
	queueRemove( q );
	if ( ! solved )
		printf("Function queueRemove has not been implemented.\n");
}

void use_queue_get ( tQueue* q ) {
	solved = 1;
	err_flag = 0;
	char c;
	queueGet( q, &c );
	if ( ! solved )
		printf("Function queueGet has not been implemented.\n");
	else if ( !err_flag )
		printf("Function queueGet returned value '%c'.\n", c );
}

void use_queue_up ( tQueue* q, char c ) {
    solved = 1;
    queueUp( q, c );
    if ( ! solved )
		    printf("Function queueUp has not been implemented.\n");
		return;
}

/*******************************************************************************
 * ADVANCED TESTS
 ******************************************************************************/

int main ( int argc, char* argv[] ) {

	printf ("**************************************************\n");
	printf ("* C203: Queue implemented using an array         *\n");
	printf ("* Advamced testing script (c203-advanced-test.c) *\n");
	printf ("**************************************************\n");
	printf ("* The explanation:                               *\n");
	printf ("* ~~~~~~~~~~~~~~~~                               *\n");
	printf ("* ? - not used yet                               *\n");
	printf ("* F - Front                                      *\n");
	printf ("* B - Back                                       *\n");
	printf ("* E - Empty (F == B)                             *\n");
	printf ("**************************************************\n");

	QUEUE_SIZE = 9;
                                     /* We allocate the memory for the queue. */
    queue = (tQueue*)malloc(sizeof(tQueue));
    if ( queue == NULL ) {
    	printf ("\nc203-test.c: Error: There is not enough memory for the queue.\n");
        return 1;
    }

	printf ("\n[TEST01] queue initialization\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
    use_queue_init( queue );
	queuePrint( queue );

	printf ("\n[TEST02] We try queueEmpty and queueFull over the empty queue.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_empty( queue );
	use_queue_full( queue );

	printf ("\n[TEST03] queueFront, queueGet, and queueRemove should cause handled errors.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_front ( queue );
	use_queue_get ( queue );
	use_queue_remove ( queue );
	queuePrint( queue );
                                     /* Test 04 is additional to basic tests. */
	printf ("\n[TEST04] We check queueEmpty and queueFull.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_empty( queue );
	use_queue_full( queue );

	printf ("\n[TEST05] We put 'Ciao!' into the queue using queueUp.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_up( queue, 'C' );
	use_queue_up( queue, 'i' );
	use_queue_up( queue, 'a' );
	use_queue_up( queue, 'o' );
	use_queue_up( queue, '!' );
	queuePrint( queue );

	printf ("\n[TEST06] Functions queueEmpty as well as queueFull should return FALSE now.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_empty( queue );
	use_queue_full( queue );

	printf ("\n[TEST07] Function queueFront should always return the same value.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_front( queue );
	use_queue_front( queue );
	queuePrint( queue );

	printf ("\n[TEST08] Let's examine the difference between queueRemove and queueGet.\n" );
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_remove( queue );
	use_queue_get( queue );
	queuePrint( queue );

	printf ("\n[TEST09] After three queueGet the queue should be empty.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_get( queue );
	use_queue_get( queue );
	use_queue_get( queue );
	use_queue_empty( queue );
	use_queue_full( queue );
	queuePrint( queue );
                   /* Back index should be at the end of array after test 10. */
	printf ("\n[TEST10] We put 1, 2, and 3 to the queue using QueueUp and check the result.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_up( queue, '1' );
	use_queue_up( queue, '2' );
	use_queue_up( queue, '3' );
	use_queue_front ( queue );
	use_queue_empty( queue );
	use_queue_full( queue );
	queuePrint( queue );

	printf ("\n[TEST11] After removing three elements using queueGet, the queue should be empty.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_get( queue );
	use_queue_get( queue );
	use_queue_get( queue );
	use_queue_front ( queue );
	use_queue_empty( queue );
	use_queue_full( queue );
	queuePrint( queue);

	printf ("\n[TEST12] Additional queueUp should move to b_index possition 0.\n" );
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_up( queue, '4' );
	use_queue_front ( queue );
	use_queue_empty( queue );
	use_queue_full( queue );
	queuePrint( queue );

	printf ("\n[TEST13] After queueGet, the indexes should be the same as after queueInit.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_get( queue );
	use_queue_front ( queue );
	use_queue_empty( queue );
	use_queue_full( queue );
	queuePrint( queue);

	printf ("\n[TEST14] We use queueUp eight times to fill up the queue.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	for ( int i=0; i<8; i++ )
		use_queue_up( queue, 'A'+i );
	use_queue_front ( queue );
	use_queue_empty( queue );
	use_queue_full( queue );
	queuePrint( queue );

	printf ("\n[TEST15] Addition queueUp should cause hadled error.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_up( queue, 'Z' );
	use_queue_front ( queue );
	use_queue_empty( queue );
	use_queue_full( queue );
	queuePrint( queue );
                              /* The rest of test is not part of basic tests. */
	printf ("\n[TEST16] Many couples of queueUp and queueRemove operations.\n" );
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	for ( char c='a'; c<='z'; c++ ) {
		use_queue_remove( queue );
		use_queue_up ( queue, c );
	}
	queuePrint( queue );
	use_queue_empty( queue );
	use_queue_full( queue );

	printf ("\n[TEST17] We create the queue of the size 2.\n");
	printf ("===========================================\n");
	QUEUE_SIZE = 3;
    use_queue_init( queue );
	queuePrint( queue );

	printf ("\n[TEST18] Let's test all operations which should not change empty queue.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_empty ( queue );
	use_queue_full ( queue );
	use_queue_front ( queue );                                      /* Error. */
	use_queue_get ( queue );                                        /* Error. */
	use_queue_remove ( queue );                                     /* Error. */
	queuePrint ( queue );

	printf ("\n[TEST19] Calling queueUp three times should cause error.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_up ( queue, '1' );
	use_queue_up ( queue, '2' );
	use_queue_up ( queue, '3' );
	use_queue_empty ( queue );
	use_queue_full ( queue );
	use_queue_front ( queue );
	use_queue_get ( queue );
	queuePrint ( queue );

	printf ("\n[TEST20] Calling queueRemove followed by queueGet should cause error.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_remove ( queue );
	use_queue_get ( queue );
	use_queue_empty ( queue );
	use_queue_full ( queue );
	use_queue_front ( queue );
	queuePrint ( queue );

	printf ("\n[TEST21] We check the nextIndex function by two queueUp and queueGet.\n");
	printf ("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	use_queue_up ( queue , '3');
	use_queue_get ( queue );
	use_queue_up ( queue , '4');
	use_queue_get ( queue );
	use_queue_empty ( queue );
	use_queue_full ( queue );
	use_queue_front ( queue );
	queuePrint ( queue );
                                                           /* Dirty trick :-) */
	printf ("\n[TEST22] And finally, we create the queue of size 0.\n");
	printf ("====================================================\n");
    QUEUE_SIZE = 1;
	queueInit ( queue );
	queuePrint ( queue );
	use_queue_empty ( queue );
	use_queue_full ( queue );
	use_queue_front ( queue );
	use_queue_get ( queue );
	use_queue_remove ( queue );
	queuePrint( queue );

	use_queue_up ( queue, '1' );
	use_queue_empty ( queue );
	use_queue_full ( queue );
	use_queue_front ( queue );
	use_queue_get ( queue );
	use_queue_remove ( queue );
	queuePrint ( queue );

	use_queue_empty ( queue );
	use_queue_full ( queue );
	use_queue_front ( queue );

	printf ("\n******* The end of Advanced Tests (c203-advanced-test.c) *******\n");
	free (queue);
	return 0;
}
/**/