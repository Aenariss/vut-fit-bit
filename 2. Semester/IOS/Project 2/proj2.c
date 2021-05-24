/* 
IOS PROJEKT 2 
AUTHOR: Vojtech Fiala 
LOGIN: xfiala61
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h> 
#include <sys/mman.h>
#include <semaphore.h> 
#include <fcntl.h>
#include <time.h>
#include <errno.h>

//map shared memory
void MMAP(int **pointer) {
    *pointer = (int*)mmap(NULL, sizeof(*pointer), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

//Remve shared memory
void MUNMAP(int **pointer) {
    munmap(*pointer, sizeof(*pointer));
}

//declare variables
int *NE = 0;
int *NB = 0;
int *NC = 0;
int *global_counter = NULL;
int *judge = 0;
int *immigrant = NULL;
int *entered = 0;
int *checked = 0;
int *immigrant_confirmed = 0;
int *migrant_counter = 0;
sem_t *judge_in_the_house;
sem_t *mutex;
sem_t *allSigned;
sem_t *confirmed;
sem_t *data_write;
sem_t *allConfirmed;
sem_t *judge_edge;
FILE *filePointer;

//function to initialize semaphores & shared memory
int initialize() {
    errno=0;
    if ((judge_in_the_house = sem_open("/xfiala61.judge", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        fprintf(stderr,"Semaphore initialization error\n");
        exit(1);
    }
    if ((mutex = sem_open("/xfiala61.mutex", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        fprintf(stderr,"Semaphore initialization error\n");
        exit(1);
    }
    if ((allSigned = sem_open("/xfiala61.allSigned", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        fprintf(stderr,"Semaphore initialization error\n");
        exit(1);
    }
    if ((confirmed = sem_open("/xfiala61.confirmed", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        fprintf(stderr,"Semaphore initialization error\n");
        exit(1);
    }
    if ((data_write = sem_open("/xfiala61.data_write", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        fprintf(stderr,"Semaphore initialization error\n");
        exit(1);
    }
    if ((judge_edge = sem_open("/xfiala61.judge_edge", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        fprintf(stderr,"Semaphore initialization error\n");
        exit(1);
    }
    MMAP(&NE);
    MMAP(&NB);
    MMAP(&migrant_counter);
    MMAP(&NC);
    MMAP(&global_counter);
    MMAP(&judge);
    filePointer = fopen("proj2.out", "w");
    if (filePointer == NULL) {
        fprintf(stderr, "Could not open file\n");
        exit(1);
    }
    setbuf(filePointer, NULL);
    return 0;
}

//function to remove shared memory & close the semaphores
int uninitialize() {
    MUNMAP(&global_counter);
    MUNMAP(&judge);
    MUNMAP(&NE);
    MUNMAP(&NB);
    MUNMAP(&NC);
    MUNMAP(&migrant_counter);
    fclose(filePointer);
    sem_close(judge_in_the_house);
    sem_unlink("/xfiala61.judge");
    sem_close(mutex);
    sem_unlink("/xfiala61.mutex");
    sem_close(allSigned);
    sem_unlink("/xfiala61.allSigned");
    sem_close(confirmed);
    sem_unlink("/xfiala61.confirmed");
    sem_close(data_write);
    sem_unlink("/xfiala61.data_write");
    sem_close(judge_edge);
    sem_unlink("/xfiala61.judge_edge");
    return 0;
}

//Print this if arguments are not valid
int argument_error() {
    fprintf(stderr, "Invalid argument(s)!\n");
    return 1;
}

int arg_check(long PI, long IG, long JG, long IT, long JT) {            //Test correct argument values
    if (PI < 1) { //At least 1 immigrant neccesary
        return argument_error();
    }
    if ((IG < 0 || JG < 0 || IT < 0 || JT < 0) || (IG > 2000 || JG > 2000 || IT > 2000 || JT > 2000)) { //Wait time can neither exceed nor be beyond these values
        return argument_error();
    }
    return 0;
}

//Wait random time
void wait_time(long value) {
    if (value > 0) {
        useconds_t sleep_time = (useconds_t) (value / (float) RAND_MAX * rand() * 1000); //get a random number of microseconds
        usleep(sleep_time);
    }
}

void immigrant_action(long IT, int immigrant_number, long JG, long IG) {
    
    //imigrant starts
    sem_wait(data_write);
    fprintf(filePointer, "%d   : IMM %d    : starts\n", ++(*global_counter), immigrant_number);
    sem_post(data_write);
    
    //immigrant enters
    sem_wait(judge_in_the_house);
    sem_wait(data_write);
    fprintf(filePointer, "%d   : IMM %d    : enters: %d: %d: %d\n", ++(*global_counter), immigrant_number, ++(*NE), *NC, ++(*NB));
    
    if ((JG == 0) || (IG == 0)) {
        sem_post(judge_edge); 
    } //edge case fix

    sem_post(data_write);
    sem_post(judge_in_the_house);
    

    // immigrant checks
    sem_wait(data_write);
    fprintf(filePointer, "%d   : IMM %d    : checks: %d: %d: %d\n", ++(*global_counter),immigrant_number,*NE,++(*NC),*NB);
    ++(*migrant_counter);
    sem_post(data_write);

    //signal judge
    sem_wait(mutex);
        if ((*judge == 1) && (*NE == *NC)) {
        sem_post(allSigned);
    }
    else {
        sem_post(mutex);
    }

    // wait for judge to confirm the applications
    sem_wait(confirmed);
    sem_wait(data_write);
    fprintf(filePointer, "%d   : IMM %d    : wants certificate: %d: %d: %d\n", ++(*global_counter), immigrant_number, *NE,*NC,*NB);
    sem_post(data_write);

    wait_time(IT); // wait a bit

    //pick up the certificate
    sem_wait(data_write);
    fprintf(filePointer, "%d   : IMM %d    : got certificate: %d: %d: %d\n", ++(*global_counter), immigrant_number, *NE,*NC,*NB);
    sem_post(data_write);

    //leave
    sem_wait(judge_in_the_house);
    sem_wait(data_write);
    fprintf(filePointer, "%d   : IMM %d    : leaves: %d: %d: %d\n", ++(*global_counter), immigrant_number, *NE,*NC,--(*NB));
    sem_post(data_write);
    sem_post(judge_in_the_house);
}

//function to generate immigrants
void Immigrant(long PI, long IG, long IT, long JG) {
    int immigrant_number[PI];
    int immigrant_amount = PI;
    for (int i = 0; i < immigrant_amount; i++) {
        wait_time(IG);
        pid_t immigrants = fork();
        if (immigrants == 0) {
            immigrant_number[i] = (i+1);
            immigrant_action(IT,immigrant_number[i], JG, IG);
            exit(0);
        }
        else if (immigrants < 0) {
            fprintf(stderr, "Fork problem!\n");
            break;
            exit(1);
        }
    waitpid(immigrants, NULL, 0);
    }
}

//Judge function
void Judge(long JG, long JT, long PI, long IG) {
    do {
    wait_time(JG);
    if ((JG == 0) || (IG == 0)) { 
        sem_wait(judge_edge); 
    } //failsafe for a situation when JG is 0
    //Check if judge can enter
    sem_wait(judge_in_the_house);
    sem_wait(mutex);

    //Judge starts his entering process
    sem_wait(data_write);
    fprintf(filePointer, "%d   : JUDGE    : wants to enter\n", ++(*global_counter));
    *judge=1;
    sem_post(data_write);

    //Judge is inside
    sem_wait(data_write);
    fprintf(filePointer, "%d   : JUDGE    : enters: %d: %d: %d\n", ++(*global_counter), *NE, *NC, *NB);
    sem_post(data_write);

    //If some migrants havent checked in yet, wait for them
    if (*NE != *NC) {
        sem_wait(data_write);
        fprintf(filePointer, "%d   : JUDGE    : waits for imm: %d: %d: %d\n", ++(*global_counter), *NE, *NC, *NB);
        sem_post(data_write);
        sem_post(mutex);
        sem_wait(allSigned);
    }

    //Confirm their applications
    sem_wait(data_write);
    fprintf(filePointer, "%d   : JUDGE    : starts confirmation: %d: %d: %d\n", ++(*global_counter), *NE, *NC, *NB);
    wait_time(JT);
    for (int number_of_checked = *NC; number_of_checked > 0; number_of_checked--) sem_post(confirmed);
    fprintf(filePointer, "%d   : JUDGE    : ends confirmation: %d: %d: %d\n", ++(*global_counter), *NE=0, *NC=0, *NB);
    sem_post(data_write);

    //Take a break - leave and repeat
    wait_time(JT);
    sem_wait(data_write);
    fprintf(filePointer, "%d   : JUDGE    : leaves: %d: %d: %d\n", ++(*global_counter), *NE, *NC, *NB);
    *judge=0;
    sem_post(data_write);

    sem_post(mutex);
    sem_post(judge_in_the_house);
    } while(*migrant_counter != PI); 
    //Finish
    sem_wait(data_write);
    fprintf(filePointer, "%d   : JUDGE    : finishes\n", ++(*global_counter));
    sem_post(data_write);
}

int main (int argc, char *argv[]) {


    if (argc != 6) {                //Check the number of arguments first
        return argument_error();
    }

    char *ptr;
    long PI = strtol(argv[1], &ptr, 10);
    if (argv[1] == ptr) {                   //Test for strtol errors (if the argument wasnt a number in the first place)
        return argument_error();
    }
    long IG = strtol(argv[2], &ptr, 10);
    if (argv[2] == ptr) {
        return argument_error();
    }
    long JG = strtol(argv[3], &ptr, 10);
    if (argv[3] == ptr) {
        return argument_error();
    }
    long IT = strtol(argv[4], &ptr, 10);
    if (argv[4] == ptr) {
        return argument_error();
    }
    long JT = strtol(argv[5], &ptr, 10);
    if (argv[5] == ptr) {
        return argument_error();
    }

    if (arg_check(PI, IG, JG, IT, JT)) {
        return 1;
    }
    srand(time(0));
    initialize();
    pid_t immigrant_pid = fork();
    if (immigrant_pid == 0) {
        Immigrant(PI, IG, IT, JG);
        exit(0);
    }
    else if (immigrant_pid < 0) {
        fprintf(stderr, "Fork error");
        exit(1);
    }
    else {
        pid_t judge_pid = fork();
        if (judge_pid == 0) {
            Judge(JG, JT, PI, IG);
            exit(0);
        }
        else if (judge_pid < 0) {
            fprintf(stderr, "Fork error");
            exit(1);
        }
        waitpid(judge_pid, NULL, 0);
    }
    waitpid(immigrant_pid, NULL, 0);
    uninitialize();
    return 0;
}