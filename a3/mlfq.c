/*
 * UVic CSC 360, Summer 2021
 * This code copyright 2021: Roshan Lasredo, Mike Zastre
 *
 * Assignment 3
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>


#include "queue.h"

///
/* 
 * Some constants related to assignment description.
 */
#define MAX_INPUT_LINE 100
#define MAX_TASKS 10
const int QUEUE_TIME_QUANTUMS[] = { 2, 4, 8 };


Queue_t *queue_1;
Queue_t *queue_2;
Queue_t *queue_3;
Task_t task_table[MAX_TASKS];
Task_t *current_task;
int remaining_quantum = 0;		// Remaining Time Quantum for the current task

void validate_args(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Invalid number of input args provided! Expected 1, received %d\n", argc - 1);
		exit(1);
	}
}

void initialize_vars() {
	queue_1 = init_queue();
	queue_2 = init_queue();
	queue_3 = init_queue();
}

void read_instruction(FILE *fp, Instruction_t *instruction) {
	char line[MAX_INPUT_LINE];
	
	if(fgets(line, sizeof(line), fp) == NULL) {
		instruction->event_tick = -1;
		instruction->is_eof = true;
		return;
	}

	int vars_read = sscanf(line, "%d,%d,%d", &instruction->event_tick, 
	&instruction->task_id, &instruction->burst_time);
	instruction->is_eof = false;

	if(vars_read == EOF || vars_read != 3) {
		fprintf(stderr, "Error reading from the file.\n");
		exit(1);
	}

	if(instruction->event_tick < 0 || instruction->task_id < 0) {
		fprintf(stderr, "Incorrect file input.\n");
		exit(1);
	}
}

Queue_t *get_queue_by_id(int queue_id) {
	switch (queue_id) {
		case 1:
			return queue_1;

		case 2:
			return queue_2;

		case 3:
			return queue_3;
	}
	return NULL;
}

void handle_instruction(Instruction_t *instruction, int tick) {
	int task_id = instruction->task_id;

	if(instruction->burst_time == 0) { 
		// New Task Arrival
		// From read_instruction we are given
		// &instruction->event_tick, 
		// &instruction->task_id, 
		// &instruction->burst_time
		// Using these variable we put them into task_table with their given id

		//Given a burst time a 0, create a Task and add it to task_table for storage as it won't be enqueued
		//but data still needs to be stored

		Task_t *task_buffer;

   		task_buffer = malloc(sizeof(Task_t));
    	task_buffer->next = NULL;

		//Default queue of a created Task would be 
		task_buffer->id = instruction->task_id;
		task_buffer->burst_time = instruction->burst_time;
		task_buffer->remaining_burst_time = instruction->burst_time;
		task_buffer->current_queue = 1;

		task_buffer->total_execution_time = 0;
		task_buffer->total_wait_time = 0;

		task_table[task_id-1] = *task_buffer;
		
		printf("[%05d] id=%04d NEW\n", tick, task_id);


	} else if(instruction->burst_time == -1) { 
		int waiting_time;
		int turn_around_time;

		//As task is terminating, set the task in task_table to null after printing necessary times
		Task_t *task_null;

   		task_null = malloc(sizeof(Task_t));
    	task_null->next = NULL;

		//Set waiting_time and turn_around_time from the task_table thats been updated from update_task_metrics and execute_task
		waiting_time = task_table[task_id-1].total_wait_time;
		turn_around_time = task_table[task_id-1].total_execution_time;
		
		printf("[%05d] id=%04d EXIT wt=%d tat=%d\n", tick, task_id, 
			waiting_time, turn_around_time);

		//Setting task to null after print to terminate
		task_table[task_id-1] = *task_null;

	} else {
		// in else, burst_time must be greater than 0, using task_id, we update the burst_time and remaining_time
		// and add it to the queue of task_table[task_id-1].current_queue, the queue will not always be placed in
		// queue 1 if burst_time finished in queue 2, it will start from there

		task_table[task_id-1].burst_time = instruction->burst_time;
		task_table[task_id-1].remaining_burst_time = instruction->burst_time;
		enqueue(get_queue_by_id(task_table[task_id-1].current_queue), &(task_table[task_id-1]));
	}
}

Task_t *peek_priority_task() {
	// Checks the queues, if queue 1 is not empty then the first Task in queue 1 has priority, if queue 1 is empty and 
	// queue 2 is not empty, 

	//If all queues are empty, then it idles and returns NULL
	if (is_empty(queue_1) && is_empty(queue_2) && is_empty(queue_3)){
		return NULL;
	// Checks if Queue 1 is empty, if not return the task	
	}else if (is_empty(get_queue_by_id(1)) == 0) {
		return (get_queue_by_id(1)->start);		
	// Checks if Queue 1 is empty and Queue 2 is occupied
	} else if ((is_empty(get_queue_by_id(2)) == 0)) {
		return (get_queue_by_id(2)->start);
	// Checks if Queue 1 and Queue 2 are empty and if Queue 3 is occupied
	} else if ((is_empty(get_queue_by_id(3)) == 0)) {
		return (get_queue_by_id(3)->start);
	}

	return NULL;
}

void decrease_task_level(Task_t *task) {
	task->current_queue = task->current_queue == 3 ? 3 : task->current_queue + 1;
}

void scheduler() {

	Task_t *priority_task = peek_priority_task();

	//If the task has run its quantum (2/4/8), decrease its task_level
	//Then check if the queue is not empty, if not enqueue it to its new (lowered) queue
	if (current_task != NULL) {	
		//Check if it's burst has reached 2
		if ((current_task->burst_time) - (current_task->remaining_burst_time) == 2){
			decrease_task_level(current_task);
			if (is_empty(get_queue_by_id(current_task->current_queue)) == 0){
				enqueue((get_queue_by_id(current_task->current_queue)), current_task);
				current_task = NULL;
			}
		//Check if it's 6 as 2 + 4, so quantum of 4 would have to run at least 6 times
		} else if ((current_task->burst_time) - (current_task->remaining_burst_time) == 6){
			decrease_task_level(current_task);
			if (is_empty(get_queue_by_id(current_task->current_queue)) == 0){
				enqueue((get_queue_by_id(current_task->current_queue)), current_task);
				current_task = NULL;
			}
		//Checks for -6 mod 8 to ensure its run 8 times in the third queue
		} else if ((((current_task->burst_time - current_task->remaining_burst_time - 6) % 8) == 0) && (current_task->total_execution_time > 6)) {
			decrease_task_level(current_task);
			if (is_empty(get_queue_by_id(current_task->current_queue)) == 0){
				enqueue((get_queue_by_id(current_task->current_queue)), current_task);
				current_task = NULL;
			}
		} 
	}

	// Checks if current_task is null and priority_task is occupied
	if (current_task == NULL && priority_task != NULL){
		//If true, then priority task dequeues from its current_queue and sets the dequeued task as current_task
		current_task = dequeue(get_queue_by_id(priority_task->current_queue));

	//Checks if current task is not null
	} else if (current_task != NULL){
		// and then checks if priority task is not null
		if (priority_task != NULL) {
			// if both tasks are not null, we check priority by looking at the current_queue's of both tasks
			// if priority_tasks queue is less than current_task, it must take priority over current_task
			if (priority_task->current_queue < current_task->current_queue){
				//Enqueues the current task into its current_queue, and then updates current task with priority task 
				enqueue((get_queue_by_id(current_task->current_queue)), current_task);
				current_task = dequeue(get_queue_by_id(priority_task->current_queue));
			}
		}
	} else {

		current_task = priority_task;
	}
}

void execute_task(int tick) {
	if(current_task != NULL) {
		// Subtract the remaining burst time by 1
		current_task->remaining_burst_time -= 1;
		// Increment the total_execution_time
		current_task->total_execution_time +=1;
		// Update the task_table with the current_task and its updated values
		task_table[(current_task->id) - 1] = *current_task;


		printf("[%05d] id=%04d req=%d used=%d queue=%d\n", tick, 
			current_task->id, current_task->burst_time, 
			(current_task->burst_time - current_task->remaining_burst_time), 
			current_task->current_queue);
		
		if(current_task->remaining_burst_time == 0) {
			current_task = NULL;
		}

	} else {
		printf("[%05d] IDLE\n", tick);
	}
}

void update_task_metrics() {

	int i;
	//set up a temporary Task_t for pop/pushing from the queue's
	Task_t *task_temp;
   	task_temp = malloc(sizeof(Task_t));
    task_temp->next = NULL;

	//Check if queue_1 is empty, if not then iterate through the queue and increment, total wait and total execution time
	if (is_empty(queue_1) == 0){
		for (i=0; i < queue_size(queue_1); i++){
			task_temp = dequeue(queue_1);
			task_temp->total_wait_time +=1;
			task_temp->total_execution_time += 1;
			enqueue(queue_1, task_temp);
		}
	} 

	//Check if queue_2 is empty, if not then iterate through the queue and increment, total wait and total execution time
	if (is_empty(queue_2) == 0){
		for (i=0; i < queue_size(queue_2); i++){
			task_temp = dequeue(queue_2);
			task_temp->total_wait_time +=1;
			task_temp->total_execution_time += 1;
			enqueue(queue_2, task_temp);
		}
	} 

	//Check if queue_3 is empty, if not then iterate through the queue and increment, total wait and total execution time
	if (is_empty(queue_3) == 0){
		for (i=0; i < queue_size(queue_3); i++){
			task_temp = dequeue(queue_3);
			task_temp->total_wait_time +=1;
			task_temp->total_execution_time += 1;
			enqueue(queue_3, task_temp);
		}
	}



}

int main(int argc, char *argv[]) {
	int tick = 1;
	int is_inst_complete = false;
	
	validate_args(argc, argv);
	initialize_vars();

	FILE *fp = fopen(argv[1], "r");

	if(fp == NULL) {
		fprintf(stderr, "File \"%s\" does not exist.\n", argv[1]);
		exit(1);
	}

	Instruction_t *curr_instruction = (Instruction_t*) malloc(sizeof(Instruction_t));
	
	// Read First Instruction
	read_instruction(fp, curr_instruction);

	if(curr_instruction->is_eof) {
		fprintf(stderr, "Error reading from the file. The file is empty.\n");
		exit(1);
	}

	while(true) {
		while(curr_instruction->event_tick == tick) {
			handle_instruction(curr_instruction, tick);

			// Read Next Instruction
			read_instruction(fp, curr_instruction);
			if(curr_instruction->is_eof) {
				is_inst_complete = true;
			}
		}
		
		scheduler();

		update_task_metrics();

		execute_task(tick);

		
		if(is_inst_complete && is_empty(queue_1) && is_empty(queue_2) && is_empty(queue_3) && current_task == NULL) {
			break;
		}

		tick++;
	}

	fclose(fp);
	deallocate(curr_instruction);
	deallocate(queue_1);
	deallocate(queue_2);
	deallocate(queue_3);
}
