# A3 CSC360 - Jason Park
# V#00946007

# Process of mlfq.c

- Multiple Functions to write: handle_instruction, peek_priority_task, scheduler, execute_task, update_task_metrics.
- Work through in that order of functions essentially acting as a line through each function.
- We know handle_instruction enqueues tasks when burst_time > 0, then peek_prioriy_task will give the reference to the task that has priority.
- With the priority_task, we can use this to set current_task and properly enqueue and dequeue.
- Update values in queue(s) with update_task_metrics by incrementing their times
- Then update remaining_burst_time and total_execution_time in execute_task.

# handle_instruction
Three conditions, burst_time == 0, -1 or > 0.
If burst_time == 0:
 - Create a task with the variables from instruction and add it to the task_table relative to its id.

If burst_time == -1:
- set print waiting times and turn around times from task_table.
- then after printing terminate the task by setting it to NULL.

If burst_time > 0:
- Update the burst_time and remaining_burst_times relative to instruction and id and then enqueue it.
- the queue will not always be placed in queue 1 if burst_time finished in queue 2, it will start from there.


# peek_priority_task

Checks the queues, if queue 1 is not empty then the first Task in queue 1 has priority, if queue 1 is empty and queue 2 is not empty, then queue 2 has priority and so forth for queue 3.
It will then return the reference to these queues with priority.
If all queues are empty, it will return null for the queue to idle until something has been enqueued into any of the queues.

# scheduler

Multiple situations with scheduler, the function first checks if the current_task has reached its quantums of 2/4/8.
If the quantum of the queue has been reached, it's task level will be lowered, then it will check if that current queue is empty. If it is then it will enqueue it to the queue.

Then it checks if current_task is null and if priority_task isn't, if true it will update current_task with a dequeue of priority_tasks current queue.

For the final check, it must be that both priority_task and current_task are not null. If both not null, it will check if the current_queue of priority is lower than current, if this is true than a queue lower than another is occupied thus taking priority. 

# execute_task
Given current_task from scheduler, update its values by subtracting remaining_burst_time and incrementing total_execution_time, then update the task_table with its ID. If the remaining_burst_time is 0, current_task will be set to NULL as that task is "terminated"

# update_task_metrics
Need to iterate through all the queues, and update their waiting times and execution times.
Check through each queue, and check if they are not empty, if occupied go through each task in the queue and increment the tasks waiting_time (as it waiting in the queue) and total_execution_time.



