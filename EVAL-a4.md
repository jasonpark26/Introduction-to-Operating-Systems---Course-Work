
# CSC 360, Summer 2021
## Assignment #4 evaluation

* Name: `Jason Park`
* VNum: `V00946007`
* Netlink: `jasonpark`

---

The evaluation of your fourth assignment has used a somewhat different
process than for the previous three. That is, earlier the teaching team
used individual tests to probe for and determine whether specific
behavior was present in submitted work.

For this assignment I (MZ) used all four of the provided memory-trace
files plus three other very short traces (all of which can be found in
`a4-testing-stuff.zip` at Brightspace) to consider the overall behavor
of simulating each of the three page-replacement schemes.

Therefore what you are now receiving is a more general evaluation
which considers how closely your simulation's results match expected
and  reasonable behavior. (Because I provided a loose specification of
CLOCK, it is possible that some acceptable implementations have worse
page-fault behavior than FIFO, but not much worse.)

Additional comments on and observations about your
submitted code may also be provided.

In general I was very pleased with the effort puts into the
submissions by students this semester. Thank you!

---
### Preliminaries

* `virtmem.c` submitted and compiles : **Yes**
* `README.md` (or equivalent) provided which is also helpful: **Submitted. Very clearly written. Thank you!**


---
### Sanity check: swap-out

* Checking if very basic swap-out implementation is present: **Passed**


### Sanity check: Basic FIFO and LFU

* Checking if FIFO implementation is correct with a very simple test case: **Passed**
* Checking if LRU implementation is correct with a very simple test case: **Passed**


---

### Using four memory-trace files

* Using `hello-trace.out` for `FIFO`, `LFU`, and `CLOCK: **Clock is significantly worse that FIFO, although much better than LFU. (Clock's behavior may be due to assumptions made about clock-hand distances.) Sometime seems wrong, however, with the number of swapouts -- they are far, far higher than would be expected (i.e., they should not be nearly equal to the number of swapins).**
* Using `hello-py-trace.out` for `FIFO`, `LFU`, and `CLOCK: **Clock is significantly worse that FIFO, although much better than LFU. (Clock's behavior may be due to assumptions made about clock-hand distances.) Sometime seems wrong, however, with the number of swapouts -- they are far, far higher than would be expected (i.e., they should not be nearly equal to the number of swapins).**
* Using `gcc-hello-trace.out` for `FIFO`, `LFU`, and `CLOCK: **Clock is significantly worse that FIFO, although much better than LFU. (Clock's behavior may be due to assumptions made about clock-hand distances.) Sometime seems wrong, however, with the number of swapouts -- they are far, far higher than would be expected (i.e., they should not be nearly equal to the number of swapins).**
* Using `sort-1000-integers-trace.out` for `FIFO`, `LFU`, and `CLOCK: **Clock is significantly worse that FIFO, although much better than LFU. (Clock's behavior may be due to assumptions made about clock-hand distances.) Sometime seems wrong, however, with the number of swapouts -- they are far, far higher than would be expected (i.e., they should not be nearly equal to the number of swapins).**

---
### Other comments

* Any other comments: **Have a look at the test case that I used for checking out LFU (i.e. `test-3.sh` in the `a4-testing-stuff.zip` file available on Brightspace). Did you experiment at all with different parameters when implementing CLOCK (i.e., distance between hands)?**

---
### Grade for submitted work (either letter or out of 100)
* **B**

