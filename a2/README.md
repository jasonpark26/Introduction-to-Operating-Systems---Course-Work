# A2 CSC360 - Jason Park
# V#00946007
## _Part A_

# Process of vaccine.c

- Three Functions to write reg_desk, vac_station, initialize_vars
- One Thread for reg_desk, "X" amount of threads for "X" amount of vac_stations given in args, where "X" <= 10
- Create pthread for reg_desk individually, while creating pthreads in loop for vac_station
- Set up the "x" vac_stations so they are waiting for persons/struct to enter queue by waiting
- As persons/struct are created in reg_desk after being enqueued, this will signal to vac_station
- So as a person/struct enters a queue, as long as they are first in with an open station, once they dequeue it will sleep and once sleep finishes signal that it will recieve another person/struct into vac_station
- This will keep happening until everyone has finished their vaccines and will exit

# reg_desk
Lock the mutex before the given code, then create a person/struct with the given variables from the text files.
Enqueue the person/struct into the queue, and increment count, (count will keep track of queue's and enqueue's).
This will signal to the waiting threads in vac_station that an object has entered the queue and print as well.
After the given while loop, this means that all persons have entered the queue and signal that everyone has been vaccinated

# vac_station
In the while loop, while the queue is not empty, signal to wait on the receiving struct/person.
After wait has been signaled, inside the loop creates a buffer for person/struct which will hold the info from the object that is dequeued
When the person is dequeued, the count will decrement make the print statements then sleep for the given time that was from ther dequeued object
It will then check if count has reached 0 and exit out of the loop if so
At the end unlock the mutex

# initalize_vars
Simple way to initialize mutex, cond and the queue
> pthread_mutex_init(&queue_mutex, NULL); 
    pthread_cond_init(&queue_condvar, NULL);
    queue = init_queue();

# main
After initializing the variables, Takes in argv[1] as the number of vac stations and turns into an int.
Make an array holding the id's of each vac_station.
Create threads for vac_station (creates as many threads as the user inputs) and reg_desk (only one)
Afterwards join the threads and clean up the variables by destroying them.


## _Part B_

# Process of meetup.c

- Set up global variables (i.e., semaphores, first or last indicator, etc).
- Initialize the global variables in initialize_meetup.
- In join_meetup to solve the "boba fett" issue following closely to the reusable barrier solution from the little book of semaphores.

# initalize_meetup
Simple function to initialize semaphores, count, n1 (n), meetFL (mf), and codeword (resource_t).

# join_meetup

Again following the little book of semaphores reusable barrier solution, use the two turnstile method to account for the "boba fett problem".
If meetFL is 1 (MEET_FIRST)  and count is 0 (first to arrive) use write_resource() to copy the contents of the 
char array referred to parameter value into the codeword resource variable .
if count has reached "n/n1" (last to arrive) that was given in initialize_meetup and meetFL is 0 (MEET_LAST) copy the contents of the char array referred to parameter value into the codeword resource variable.
Once it reaches the critical point, use read_resource to have the contents be copied into	the	character array.