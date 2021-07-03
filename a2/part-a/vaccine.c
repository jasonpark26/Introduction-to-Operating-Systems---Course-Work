/*
 * UVic CSC 360, Summer 2021
 * This code copyright 2021: Roshan Lasrado, Mike Zastre
 *
 * Assignment 2: Part A
 * Name: Jason Park
 * V#00946007
 * --------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#include "queue.h"

#define MAX_VAC_STATIONS 10
#define MAX_INPUT_LINE 100
#define TENTHS_TO_SEC 100000


Queue_t *queue;
pthread_mutex_t queue_mutex;
pthread_cond_t queue_condvar;
unsigned int num_vac_stations;
unsigned int is_vac_completed = false;
//Add another counter to make sure that when something is added to the queue
//that it is also decremented as well
int count = 0;

/*
 * Function: reg_desk
 * ------------------
 *  Registration Desk Thread.
 *  Reads the input file and adds the vaccination persons to the
 *  queue as per their arrival times.
 * 
 *  arg: Input file name
 *  
 *  returns: null
 */
void *reg_desk(void *arg) {
    char *filename = (char *)arg;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "File \"%s\" does not exist.\n", filename);
        exit(1);
    }

    char line[MAX_INPUT_LINE];
    unsigned int current_time = 0;

    while (fgets(line, sizeof(line), fp)) {
        pthread_mutex_lock(&queue_mutex);
        int person_id;
        int person_arrival_time;
        int person_service_time;

        int vars_read = sscanf(line, "%d:%d,%d", &person_id, 
            &person_arrival_time, &person_service_time);

        if (vars_read == EOF || vars_read != 3) {
            fprintf(stderr, "Error reading from the file.\n");
            exit(1);
        }

        if (person_id < 0 || person_arrival_time < 0 || 
            person_service_time < 0)
        {
            fprintf(stderr, "Incorrect file input.\n");
            exit(1);
        }

        int arrival_time = person_arrival_time;
        
        // Sleep to simulate the persons arrival time.
        usleep((arrival_time - current_time) * TENTHS_TO_SEC);
        fprintf(stdout, "Person %d: arrived at time %d.\n", 
            person_id, arrival_time);

        current_time = arrival_time;

        //Using a struct, creates a person that uses the variables and puts it into the queue
        PersonInfo_t *person = new_person();       
        person->id = person_id;
	    person->arrival_time = person_arrival_time;
        person->service_time = person_service_time;
    
        //Add created struct/person to a queue and incremement count
        enqueue(queue, person);
        count++;

        fprintf(stdout, "Person %d: Added to the queue.\n", person->id);

        //Signal to threads that a person has been added to queue and to start vaccinating them
        pthread_mutex_unlock(&queue_mutex);
        pthread_cond_signal(&queue_condvar);


        pthread_mutex_unlock(&queue_mutex);
    }

    fclose(fp);

    is_vac_completed = true;
    pthread_cond_broadcast(&queue_condvar);

    return NULL;
}

/*
 * Function: vac_station
 * ---------------------
 *  Vaccination Station Thread.
 *  Vaccinate the persons from the queue as per their service times.
 *
 *  arg: Vaccination station number
 *  returns: null
 *
 */

void *vac_station(void *arg) {
    int station_num = *((int *)arg);

    while (true) {
        pthread_mutex_lock(&queue_mutex);

        //while the queue is not empty, signal to wait on the receiving struct/person
        while (is_empty(queue) != 0){
            pthread_cond_wait(&queue_condvar, &queue_mutex);
        } 

        //create a buffer for a person to get their id from the dequeue, then decrement count
        PersonInfo_t *person_buffer = new_person();  
        person_buffer = dequeue(queue);
        count--;

        fprintf(stdout, "Vaccine Station %d: START Person %d Vaccination.\n", station_num, person_buffer->id);

        //sleep for the given service time
        usleep(person_buffer->service_time * TENTHS_TO_SEC);
        
        fprintf(stdout, "Vaccine Station %d: FINISH Person %d Vaccination.\n", station_num, person_buffer->id);

        // Once count has reached 0, break from the infinite loop to signal end of thread
        if (count == 0) {
            break;
        }
        pthread_mutex_unlock(&queue_mutex); 
    }
    return NULL;
}

void validate_args(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid number of input args provided! "
                        "Expected 2, received %d\n", 
                        argc - 1);
        exit(1);
    }

    num_vac_stations = atoi(argv[1]);
    if ((num_vac_stations <= 0) || (num_vac_stations > MAX_VAC_STATIONS)) {
        fprintf(stderr, "Vaccine stations exceeded the MAX LIMIT.%d\n", 
            argc - 1);
        exit(1);
    }

}

/*
 * Function: initialize_vars
 * -------------------------
 *  Initialize the mutex, conditional variable and the queue.
 */

void initialize_vars() {
    pthread_mutex_init(&queue_mutex, NULL); 
    pthread_cond_init(&queue_condvar, NULL);
    queue = init_queue();
}


/*
 * Function: main
 * -------------------------
 * Code from https://code-vault.net/lesson/tlu0jq32v9:1609364042686 to set up main (i.e. looping to create threads)
*/
int main(int argc, char *argv[]) {
    int i, status;

    validate_args(argc, argv);

    initialize_vars();

    //Takes in argv[1] as the number of vac stations and turns into int
    //Takes in argv[2] as the text file
    num_vac_stations = atoi(argv[1]); 
    int stations[MAX_VAC_STATIONS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    //create threads for vac_station (creates as many threads as the user inputs) and reg_desk (only one)
    pthread_t th[num_vac_stations];
    for (i = 0; i < (num_vac_stations); i++) {
        if (pthread_create(&th[i], NULL, vac_station, &stations[i]) != 0) {
            perror("Failed to create thread");
        }
    }
    if (pthread_create(&th[num_vac_stations], NULL, reg_desk, argv[2]) != 0) {
        perror("Failed to create thread");
    }
    
    //join threads
    for (i = 0; i < (num_vac_stations); i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        } 
    }
    if (pthread_join(th[num_vac_stations], NULL) != 0) {
        perror("Failed to join thread");
    }

    //Clean up mutex and cond by destroy
    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&queue_condvar);
    exit(0);
}
