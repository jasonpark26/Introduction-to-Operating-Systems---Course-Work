# A3 CSC360 - Jason Park
# V#00946007


# Process of virtmem.c

- Create Global Variables and modify page_table struct to help with replacement schemes
- Check if memwrite is true and set the dirty bit when needed.
- Work on FIFO, LFU, then Clock

# FIFO
Created a global variable *fifo_track* which keeps track of the frame which will be replaced. Since FIFO is first in first out it will iterate through all the available frames in order. To account for reaching the end of frames, there is a modulo on fifo_track by the size_of_memory so it can keep going through the frames in order. Now that the effective frame is found, increment fifo_track and then compute and return the effective address.

# LFU
Modify page_table to add the variable *LFUcounter* which holds the amount of time the page in the frame has been referenced.
In resolve_address, when there is a free frame set the counter to 1, and when the page/frame is found increment counter.
In the replacement scheme create two variable *lowest* and *stored*, *lowest* holds the counter of the current least frequently referenced frame and *stored* holds the current least frequently referenced frame. Iterate through the table using size_of_memory to find the least frequently referenced frame/counter. In the event of a tie breaker, the second loop finds the frame and LFUcounter with the least frequently used. This loop iterates through the table finding the first occurence of the *lowset* to decide the tie breaker. If it finds the lowest that means the tie breaker has been found and break out of the loop. As the frame of LFU has been found as *stored*, update the table. If the frame has a dirty bit flagged, then swap_out will be incrememented. As frame is found compute and return the effective address.

# Clock
Modify page_table struct to add *hit*. Check if page is already in a table, if found been found, set the hit to 1.
Iterate through the table using size_of_memory and find the first occurence of when hit is equal to 0, if so, set clockHit to that frame. Then in another loop check the first occurence of when hit is equal to 1, if so set hit back to 0. As the frame of the clock has been found as stored, update the table. If the frame has a dirty bit flagged, then swap_out will be incrememented . As frame is found compute and return the effective address.