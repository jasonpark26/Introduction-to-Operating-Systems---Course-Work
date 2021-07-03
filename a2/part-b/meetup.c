#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include "meetup.h"
#include "resource.h"


/* Create Global Variables

#define MEET_LAST 0
#define MEET_FIRST 1
 - These are from meetup.h
*/

// Three semaphores from Little Book of Semaphores
sem_t turnstile;
sem_t turnstile2;
sem_t mutex;
int count;
//n1 acts as n
int n1;
//meetFL acts as an indicator for MEET_FIRST and MEET_LAST
int meetFL;
//codeword must be stored as an instance of resource_t
static resource_t codeword;

void initialize_meetup(int n, int mf) {
    char label[100];
    int i;

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }
    
    count = 0;
    //n1 is set to n
    n1 = n;
    //meetFL is set to what mf is (0 or 1)
    meetFL = mf;

    //from resource.h initalize codeword= using init_resource
    init_resource(&codeword, "temp codeword");

    // initalize the semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&turnstile, 0, 0);
    sem_init(&turnstile2, 0, n1);
}


// Copied the formatting of the reusable barrier solution from the little book of semaphores

void join_meetup(char *value, int len) {

    sem_wait(&mutex);

    // if meetFL is 1 (MEET_FIRST)  and count is 0 (first to arrive) use write_resource() to copy the contents of the 
    // char array referred to parameter value into the codeword resource variable 
    if ((count == 0) && (meetFL == 1)) {
        write_resource(&codeword, value, len);      
    }
        count += 1;

    // if count has reached "n/n1" (last to arrive) that was given in initialize_meetup cont.
    if (count == n1){
        // and meetFL is 0 (MEET_LAST) copy the contents of the char array referred to parameter value into the codeword resource variable.
        if (meetFL == 0) {
            write_resource(&codeword, value, len);
        }
        //lock the second
        sem_wait(&turnstile2); 
        //unlock the first
    	sem_post(&turnstile);
    }
    sem_post(&mutex);

    // first turnstile
    sem_wait(&turnstile);
    sem_post(&turnstile);

    //critical point
    //use read_resource to have the contents be copied into	the	character array
    read_resource(&codeword, value, len);


    sem_wait(&mutex);
        count -= 1;
        if (count == 0) {
            //lock the first
            sem_wait(&turnstile); 
            //unlock the second
    	    sem_post(&turnstile2);
        }
    sem_post(&mutex);

    //second turnstile
    sem_wait(&turnstile2);
    sem_post(&turnstile2);
}
